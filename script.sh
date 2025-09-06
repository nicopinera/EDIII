# git pull

python3 -m venv venv

source venv/bin/activate #Entrando al entorno virtual

pre-commit clean && pre-commit install && pre-commit run --all-files

deactivate # Saliendo del entorno
