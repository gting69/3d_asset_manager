from sqlalchemy import Column, Integer, String, ForeignKey, DateTime, func
from sqlalchemy.orm import relationship, declarative_base

Base = declarative_base()

class Vehicle(Base):
    __tablename__ = "vehicles"
    id = Column(Integer, primary_key=True)
    name = Column(String, unique=True, nullable=False)
    versions = relationship("VehicleVersion", back_populates="vehicle")

class VehicleVersion(Base):
    __tablename__ = "vehicle_versions"
    id = Column(Integer, primary_key=True)
    version_name = Column(String, nullable=False)
    vehicle_id = Column(Integer, ForeignKey("vehicles.id"))
    vehicle = relationship("Vehicle", back_populates="versions")
    global_parts = relationship("GlobalPart", back_populates="version")

class GlobalPart(Base):
    __tablename__ = "global_parts"
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    version_id = Column(Integer, ForeignKey("vehicle_versions.id"))
    version = relationship("VehicleVersion", back_populates="global_parts")
    components = relationship("Component", back_populates="global_part")

class Component(Base):
    __tablename__ = "components"
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    global_part_id = Column(Integer, ForeignKey("global_parts.id"))
    global_part = relationship("GlobalPart", back_populates="components")
    revisions = relationship("ComponentRevision", back_populates="component")

class ComponentRevision(Base):
    __tablename__ = "component_revisions"
    id = Column(Integer, primary_key=True)
    revision_number = Column(String, nullable=False)
    created_at = Column(DateTime, default=func.now())
    author = Column(String)
    component_id = Column(Integer, ForeignKey("components.id"))
    component = relationship("Component", back_populates="revisions")
    files = relationship("FileEntry", back_populates="revision")

class FileEntry(Base):
    __tablename__ = "file_entries"
    id = Column(Integer, primary_key=True)
    file_format = Column(String, nullable=False)
    file_size = Column(Integer)
    checksum = Column(String)
    onedrive_link = Column(String, nullable=False)
    revision_id = Column(Integer, ForeignKey("component_revisions.id"))
    revision = relationship("ComponentRevision", back_populates="files")