import typer
import utils
import os
import signal

app = typer.Typer()

@app.command()
def all():
    filepath = utils.armadillo_pids_path
    pids = open(filepath, "r")
    for pid in pids.readlines():
        os.kill(int(pid.replace("\n", "")), signal.SIGTERM)

if __name__ == "__main__":
    app()