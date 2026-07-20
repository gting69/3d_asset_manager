import os
import typer
from sqlalchemy.orm import Session
from database.database import SessionLocal
from database.models import Vehicle, VehicleVersion, GlobalPart, Component, ComponentRevision, FileEntry

app = typer.Typer()

@app.command()
def add_vehicle(name: str):
    db = SessionLocal()
    try:
        new_vehicle = Vehicle(name=name)
        db.add(new_vehicle)
        db.commit()
        typer.echo(f"Auto '{name}' Successfully added to the catalog!")
    except Exception as e:
        typer.echo(f"Error: {e}")
        db.rollback()
    finally:
        db.close()


@app.command()
def import_models(path: str = "../storage"):
    db = SessionLocal()
    typer.echo(f"Starting folder scan: {path}")

    if not os.path.exists(path):
        typer.echo(f"Error: Folder '{path}' not found!")
        return

    try:
        vehicle = db.query(Vehicle).filter_by(name="Imported_UAV").first()
        if not vehicle:
            vehicle = Vehicle(name="Imported_UAV")
            db.add(vehicle)
            db.flush()

        version = db.query(VehicleVersion).filter_by(version_name="v1.0", vehicle_id=vehicle.id).first()
        if not version:
            version = VehicleVersion(version_name="v1.0", vehicle_id=vehicle.id)
            db.add(version)
            db.flush()

        files_added = 0

        for root, dirs, files in os.walk(path):
            for file_name in files:
                if file_name.lower().endswith(('.stp', '.step', '.stl')):

                    base_name, ext = os.path.splitext(file_name)
                    format_type = ext.lower().replace('.', '').upper()

                    parts = base_name.split(' ', 1)
                    part_name = parts[0] if len(parts) > 0 else "Unknown_Part"
                    comp_name = parts[1] if len(parts) > 1 else base_name

                    global_part = db.query(GlobalPart).filter_by(name=part_name, version_id=version.id).first()
                    if not global_part:
                        global_part = GlobalPart(name=part_name, version_id=version.id)
                        db.add(global_part)
                        db.flush()

                    component = db.query(Component).filter_by(name=comp_name, global_part_id=global_part.id).first()
                    if not component:
                        component = Component(name=comp_name, global_part_id=global_part.id)
                        db.add(component)
                        db.flush()

                    revision = db.query(ComponentRevision).filter_by(revision_number="v1",
                                                                     component_id=component.id).first()
                    if not revision:
                        revision = ComponentRevision(revision_number="v1", author="AutoImporter",
                                                     component_id=component.id)
                        db.add(revision)
                        db.flush()

                    file_path = os.path.join(root, file_name).replace("\\", "/")
                    file_size = os.path.getsize(os.path.join(root, file_name))

                    existing_file = db.query(FileEntry).filter_by(onedrive_link=file_path).first()
                    if not existing_file:
                        file_entry = FileEntry(
                            file_format=format_type,
                            file_size=file_size,
                            onedrive_link=file_path,
                            revision_id=revision.id
                        )
                        db.add(file_entry)
                        files_added += 1

        db.commit()
        typer.echo(f"Scanning complete. Added {files_added} new files.")

    except Exception as e:
        db.rollback()
        typer.echo(f"Error during import: {e}")
    finally:
        db.close()

if __name__ == "__main__":
    app()