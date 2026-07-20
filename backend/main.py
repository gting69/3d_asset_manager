from fastapi import FastAPI, Depends
from fastapi.responses import RedirectResponse
from sqlalchemy.orm import Session
from database.database import SessionLocal
from database.models import Vehicle, VehicleVersion, GlobalPart, Component, ComponentRevision, FileEntry
import schemas

app = FastAPI(title="3D Asset Manager API")

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@app.get("/")
def read_root():
    return RedirectResponse(url="/docs")


@app.get("/vehicles", response_model=list[schemas.VehicleResponse])
def get_vehicles(db: Session = Depends(get_db)):
    return db.query(Vehicle).all()


@app.get("/vehicles/{vehicle_id}/versions", response_model=list[schemas.VehicleVersionResponse])
def get_vehicle_versions(vehicle_id: int, db: Session = Depends(get_db)):
    return db.query(VehicleVersion).filter(VehicleVersion.vehicle_id == vehicle_id).all()


@app.get("/versions/{version_id}/global-parts", response_model=list[schemas.GlobalPartResponse])
def get_global_parts(version_id: int, db: Session = Depends(get_db)):
    return db.query(GlobalPart).filter(GlobalPart.version_id == version_id).all()


@app.get("/global-parts/{part_id}/components", response_model=list[schemas.ComponentResponse])
def get_components(part_id: int, db: Session = Depends(get_db)):
    return db.query(Component).filter(Component.global_part_id == part_id).all()


@app.get("/components/{component_id}/revisions", response_model=list[schemas.ComponentRevisionResponse])
def get_revisions(component_id: int, db: Session = Depends(get_db)):
    return db.query(ComponentRevision).filter(ComponentRevision.component_id == component_id).all()


from fastapi.responses import FileResponse
import os
from fastapi import HTTPException


@app.get("/download/{file_id}")
def download_file(file_id: int, db: Session = Depends(get_db)):
    file_entry = db.query(FileEntry).filter(FileEntry.id == file_id).first()

    if file_entry:
        file_path = os.path.abspath(os.path.join("..", file_entry.onedrive_link))
        if os.path.exists(file_path):
            return FileResponse(path=file_path, filename=os.path.basename(file_path))

    print(f"File {file_id} Not found in the database. Generating a test file...")

    mock_storage_path = os.path.abspath("mock_storage")
    os.makedirs(mock_storage_path, exist_ok=True)

    mock_file_path = os.path.join(mock_storage_path, f"test_asset_v{file_id}.dat")

    if not os.path.exists(mock_file_path):
        with open(mock_file_path, "wb") as f:
            f.write(f"This is a mock 3D asset file for ID {file_id}!".encode('utf-8'))

    return FileResponse(
        path=mock_file_path,
        filename=f"asset_v{file_id}.dat",
        media_type="application/octet-stream"
    )
