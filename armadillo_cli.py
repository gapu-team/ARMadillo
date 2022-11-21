import typer

import armadillo_cli_add
import armadillo_cli_list
import armadillo_cli_delete

app = typer.Typer()
app.add_typer(armadillo_cli_add.app, name="add", help="Add new policy      (policy|policy-dns|policy-dpi)")
app.add_typer(armadillo_cli_list.app, name="list", help="List all policies   (policy|policy-dns|policy-dpi)")
app.add_typer(armadillo_cli_delete.app, name="delete", help="Delete all policies (policy|policy-dns|policy-dpi)")

if __name__ == "__main__":
    app()