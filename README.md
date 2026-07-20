# 3D Asset Manager

A Client-Server application for managing and downloading 3D vehicle assets.

## Architecture
This project implements a classic Client-Server architecture:
- Backend: Python, FastAPI, SQLAlchemy, SQLite
- Client (Desktop): C++, Qt 6, QML

## Features
- RESTful API for managing Vehicles, Versions, Global Parts, and Components.
- Automatic API documentation via Swagger UI.
- Desktop client with dynamic UI (QML ListView, StackView navigation).
- Network integration (QNetworkAccessManager) for fetching JSON data.
- Direct binary file downloading and saving to the local filesystem.

## How to Run

### 1. Start the Backend (Server)
1. Navigate to the `backend` folder.
2. Install dependencies: `pip install -r requirements.txt`
3. Run the FastAPI server: `uvicorn main:app --reload`
4. Open your browser and go to `http://127.0.0.1:8000/docs` to see the interactive API documentation.

### 2. Start the Client (Desktop App)
1. Open the `client` folder in JetBrains CLion (or Qt Creator).
2. Wait for CMake to load the project.
3. Build and Run the `appAssetManagerClient` target.
