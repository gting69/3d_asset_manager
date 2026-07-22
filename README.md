# 3D Asset Manager

A Client-Server application for managing, structuring, and downloading 3D vehicle and aircraft assets.

## 🛠 Architecture & Tech Stack
This project implements a classic Client-Server architecture:
- **Backend:** Python, FastAPI, SQLAlchemy, SQLite
- **CLI Manager:** Typer (for backend administration)
- **Client (Desktop):** C++, Qt 6, QML (Model-View architecture)

## Features
- **RESTful API:** Full management of Vehicles, Versions, Global Parts, and Components.
- **Interactive API Docs:** Automatic documentation via Swagger UI.
- **Desktop Client UI:** Dynamic interface utilizing `QML ListView` and `StackView` navigation.
- **Real-Time Search:** Instant UI filtering via a custom C++ `QSortFilterProxyModel` (`SearchProxyModel`) and QML `TextField`, eliminating redundant database queries.
- **Hierarchical User Flow:** Seamless navigation path: *Vehicle ➔ Version ➔ Global Part ➔ Component ➔ File Download*.
- **Binary File Handling:** Direct downloading and saving of 3D assets to the local filesystem via `QNetworkAccessManager`.

## CLI Manager (`manage.py`)
An administrative backend tool designed to manage the catalog, perform CRUD operations, and handle file linking manually (resolving issues with chaotic file names during mass imports).

### Entity Management (CRUD)
- **List all vehicles:** `python manage.py vehicle list`
- **Add a new vehicle:** `python manage.py vehicle add "Audi A3"`
- **Update a vehicle:** `python manage.py vehicle update <id> "New Name"`
- **Delete a vehicle:** `python manage.py vehicle delete <id>`

### File Management
- **Smart Import:** Scans storage, automatically matches recognized files by name, and flags unrecognized files for manual processing.
  ```bash
  python manage.py file smart-import

### Manual File Linking: Link an unrecognized file directly to a specific revision.
  python manage.py file link <revision_id> "<path_to_file>"

## How to Run

### Start the Backend (Server)
1. Navigate to the backend folder.
2. Install dependencies:
   pip install -r requirements.txt
3. Run the FastAPI server:
   uvicorn main:app --reload
4. Open browser and go to http://127.0.0.1:8000/docs to see the interactive API documentation.

### Start the Client (Desktop App)
1. Open the client folder in JetBrains CLion (or Qt Creator).
2. Wait for CMake to load the project dependencies.
3. Build and Run the appAssetManagerClient target.
