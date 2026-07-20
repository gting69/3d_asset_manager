from pydantic import BaseModel
from typing import List, Optional

class VehicleBase(BaseModel):
    name: str

class VehicleResponse(VehicleBase):
    id: int

    class Config:
        from_attributes = True

class VehicleVersionBase(BaseModel):
    version_name: str

class VehicleVersionResponse(VehicleVersionBase):
    id: int
    vehicle_id: int

    class Config:
        from_attributes = True

class GlobalPartBase(BaseModel):
    name: str

class GlobalPartResponse(GlobalPartBase):
    id: int
    version_id: int

    class Config:
        from_attributes = True

class ComponentBase(BaseModel):
    name: str

class ComponentResponse(ComponentBase):
    id: int
    global_part_id: int

    class Config:
        from_attributes = True

class FileEntryBase(BaseModel):
    file_format: str
    file_size: Optional[int]
    onedrive_link: str

class FileEntryResponse(FileEntryBase):
    id: int
    revision_id: int

    class Config:
        from_attributes = True

class ComponentRevisionBase(BaseModel):
    revision_number: str
    author: Optional[str]

class ComponentRevisionResponse(ComponentRevisionBase):
    id: int
    component_id: int
    files: List[FileEntryResponse] = []

    class Config:
        from_attributes = True