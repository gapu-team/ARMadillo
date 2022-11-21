import typer
import os

import utils
import armadillo_cli_add
import armadillo_cli_list
import armadillo_cli_delete

if not os.path.exists(utils.armadillo_dir_path):
    os.mkdir(utils.armadillo_dir_path)

app = typer.Typer()
app.add_typer(armadillo_cli_add.app, name="add", help="Add new policy      (policy|policy-dns|policy-dpi)")
app.add_typer(armadillo_cli_list.app, name="list", help="List all policies   (policy|policy-dns|policy-dpi)")
app.add_typer(armadillo_cli_delete.app, name="delete", help="Delete all policies (policy|policy-dns|policy-dpi)")

if __name__ == "__main__":
    app()