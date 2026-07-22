import os
import typer
from sqlalchemy.orm import Session
from database.database import SessionLocal
from database.models import Vehicle, VehicleVersion, GlobalPart, Component, ComponentRevision, FileEntry

app = typer.Typer(help="3D Asset Manager")

vehicle_app = typer.Typer(help="Car Management")
version_app = typer.Typer(help="Version control")
file_app = typer.Typer(help="File and Import Management")

app.add_typer(vehicle_app, name="vehicle")
app.add_typer(version_app, name="version")
app.add_typer(file_app, name="file")


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@vehicle_app.command("list")
def list_vehicles():
    db = next(get_db())
    vehicles = db.query(Vehicle).all()
    typer.echo("\nCar Catalog")
    for v in vehicles:
        typer.echo(f"ID: {v.id} | Name: {v.name}")

@vehicle_app.command("add")
def add_vehicle(name: str):
    db = next(get_db())
    new_vehicle = Vehicle(name=name)
    db.add(new_vehicle)
    db.commit()
    typer.secho(f"Auto '{name}' successfully added!", fg=typer.colors.GREEN)


@vehicle_app.command("update")
def update_vehicle(vehicle_id: int, new_name: str):
    db = next(get_db())
    vehicle = db.query(Vehicle).filter(Vehicle.id == vehicle_id).first()
    if vehicle:
        vehicle.name = new_name
        db.commit()
        typer.secho(f"Auto ID {vehicle_id} updated to '{new_name}'", fg=typer.colors.BLUE)
    else:
        typer.secho(f"Auto з ID {vehicle_id} not found.", fg=typer.colors.RED)


@vehicle_app.command("delete")
def delete_vehicle(vehicle_id: int):
    db = next(get_db())
    vehicle = db.query(Vehicle).filter(Vehicle.id == vehicle_id).first()
    if vehicle:
        db.delete(vehicle)
        db.commit()
        typer.secho(f"Auto з ID {vehicle_id} deleted.", fg=typer.colors.YELLOW)
    else:
        typer.secho(f"Auto з ID {vehicle_id} not found.", fg=typer.colors.RED)


@version_app.command("list")
def list_versions(vehicle_id: int):
    db = next(get_db())
    versions = db.query(VehicleVersion).filter(VehicleVersion.vehicle_id == vehicle_id).all()
    typer.echo(f"\nAutomotive Versions ID {vehicle_id} ")
    for v in versions:
        typer.echo(f"ID Versions: {v.id} | Name: {v.version_name}")


@version_app.command("add")
def add_version(vehicle_id: int, name: str):
    db = next(get_db())
    version = VehicleVersion(version_name=name, vehicle_id=vehicle_id)
    db.add(version)
    db.commit()
    typer.secho(f"The version '{name}' added to the auto ID {vehicle_id}!", fg=typer.colors.GREEN)


@version_app.command("delete")
def delete_version(version_id: int):
    db = next(get_db())
    version = db.query(VehicleVersion).filter(VehicleVersion.id == version_id).first()
    if version:
        db.delete(version)
        db.commit()
        typer.secho(f"The version with ID {version_id} deleted", fg=typer.colors.YELLOW)
    else:
        typer.secho(f"The version with ID {version_id} not found.", fg=typer.colors.RED)



@file_app.command("link")
def link_file_manual(revision_id: int, file_path: str):
    db = next(get_db())
    if not os.path.exists(file_path):
        typer.secho(f"File '{file_path}' not found on disk!", fg=typer.colors.RED)
        return

    base_name, ext = os.path.splitext(file_path)
    format_type = ext.lower().replace('.', '').upper()
    file_size = os.path.getsize(file_path)

    file_entry = FileEntry(
        file_format=format_type,
        file_size=file_size,
        onedrive_link=file_path.replace("\\", "/"),
        revision_id=revision_id
    )
    db.add(file_entry)
    db.commit()
    typer.secho(f"File '{os.path.basename(file_path)}' successfully linked to the revision ID {revision_id}!",
                fg=typer.colors.GREEN)


@file_app.command("smart-import")
def smart_import(path: str = "../storage"):
    db = next(get_db())
    typer.echo(f"Starting folder scan: {path}")

    if not os.path.exists(path):
        typer.secho(f"The folder '{path}' not found!", fg=typer.colors.RED)
        return

    unrecognized_files = []
    files_added = 0

    for root, dirs, files in os.walk(path):
        for file_name in files:
            if file_name.lower().endswith(('.stp', '.step', '.stl')):
                file_path = os.path.join(root, file_name).replace("\\", "/")

                existing_file = db.query(FileEntry).filter_by(onedrive_link=file_path).first()
                if existing_file:
                    continue


                base_name = os.path.splitext(file_name)[0]
                matched_component = db.query(Component).filter(Component.name.ilike(f"%{base_name}%")).first()

                if matched_component:
                    revision = db.query(ComponentRevision).filter_by(component_id=matched_component.id).first()
                    if revision:
                        file_size = os.path.getsize(file_path)
                        format_type = os.path.splitext(file_name)[1].lower().replace('.', '').upper()

                        file_entry = FileEntry(
                            file_format=format_type,
                            file_size=file_size,
                            onedrive_link=file_path,
                            revision_id=revision.id
                        )
                        db.add(file_entry)
                        files_added += 1
                        typer.secho(f"Auto-link: {file_name} tied to '{matched_component.name}'",
                                    fg=typer.colors.GREEN)
                        continue

                unrecognized_files.append(file_path)

    db.commit()
    typer.echo(f"\nImport results")
    typer.echo(f"Recognized files added: {files_added}")

    if unrecognized_files:
        typer.secho("\nUnrecognized files (require manual sorting):", fg=typer.colors.YELLOW)
        for uf in unrecognized_files:
            typer.echo(f" - {uf}")
        typer.echo(
            "\nUse the 'python manage.py file link <revision_id> <file_path>' command to link them manually.")


if __name__ == "__main__":
    app()