import os
import secrets

from dotenv import load_dotenv
from fastapi import Depends, HTTPException, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBasic, HTTPBasicCredentials, HTTPBearer

load_dotenv()

bearer_scheme = HTTPBearer(auto_error=False)
basic_scheme = HTTPBasic(auto_error=False)


async def require_auth(
    bearer: HTTPAuthorizationCredentials | None = Depends(bearer_scheme),
    basic: HTTPBasicCredentials | None = Depends(basic_scheme),
):
    token = os.getenv("API_TOKEN", "change_me")
    vps_user = os.getenv("VPS_USER", "admin")
    vps_password = os.getenv("VPS_PASSWORD", "admin")

    if bearer and secrets.compare_digest(bearer.credentials, token):
        return True

    if basic and secrets.compare_digest(basic.username, vps_user) and secrets.compare_digest(
        basic.password, vps_password
    ):
        return True

    raise HTTPException(
        status_code=status.HTTP_401_UNAUTHORIZED,
        detail="Credenciales invalidas",
        headers={"WWW-Authenticate": "Bearer, Basic"},
    )
