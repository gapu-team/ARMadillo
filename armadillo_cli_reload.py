import subprocess
import typer
import utils
import os
import signal

import parser

app = typer.Typer()

@app.command()
def all():
#    filepath = utils.armadillo_pids_path
#    pids = open(filepath, "r")
#    for pid in pids.readlines():
#        os.kill(int(pid.replace("\n", "")), signal.SIGTERM)
#
    try:
        os.kill(subprocess.run(["bash", "-c", "ps aux | grep doca_dpi_scan | grep -v grep | awk '{print $2}'"]))
    except:
        pass
    try:
        os.kill(subprocess.run(["bash", "-c", "ps aux | grep doca_flow_drop | grep -v grep | awk '{print $2}'"]))
    except:
        pass
    try:
        os.kill(subprocess.run(["bash", "-c", "ps aux | grep doca_dns_filter | grep -v grep | awk '{print $2}'"]))
    except:
        pass

    parser.main()

if __name__ == "__main__":
    app()