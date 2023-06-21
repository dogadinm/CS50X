import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    transactions_db = db.execute(
        "SELECT symbol, SUM(shares), price FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    shares_total = [lookup(i['symbol'])['price']*i['SUM(shares)'] for i in transactions_db]
    total_cash = sum(shares_total) + cash

    return render_template("index.html", info=transactions_db, cash=cash, total_cash=total_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        info = lookup(symbol.upper())
        user_id = session["user_id"]
        date = datetime.datetime.now()

        if shares <= 0 or symbol == None or info == None:
            return apology("Invalid shares or number of shares ")

        total_price = shares * info["price"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        if cash < total_price:
            return apology("Not Enough Money")

        new_cash = cash - total_price

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES(?, ?, ?, ?, ?)",
                   user_id, info["symbol"], shares, info["price"], date)

        flash("Bought !")

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    return render_template("history.html", info=db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY date DESC", session["user_id"]))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        info = lookup(symbol.upper())

        if info == None:
            return apology("INVALID SYMBOL")

        return render_template("quoted.html", name=info["name"], price=info["price"], symbol=info["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology('must provide username', 403)

        elif not password:
            return apology('must provide password', 403)

        elif not confirmation:
            return apology('must provide confirmation', 403)

        elif password != confirmation:
            return apology('Passwords are not the same', 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?;", username)

        if len(rows) != 0:
            return apology('Username already exists', 403)

        hash = generate_password_hash(password)

        new_user = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        session["user_id"] = new_user

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        info = lookup(symbol.upper())
        user_id = session["user_id"]
        date = datetime.datetime.now()

        if shares <= 0 or symbol == None or info == None:
            return apology("Invalid shares or number of shares ")

        total_price = shares * info["price"]

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        user_shares = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = :id AND symbol = :symbol", id=user_id, symbol=symbol)

        user_share_real = user_shares[0]["shares"]

        if shares > user_share_real:
            return apology("You dont have this amount of shares")

        new_cash = cash + total_price

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES(?, ?, ?, ?, ?)",
                   user_id, info["symbol"], (-1)*shares, info["price"], date)

        flash("Sold !")

        return redirect("/")

    else:
        user_id = session["user_id"]
        symbols_user = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = :id GROUP by symbol HAVING SUM(shares) > 0", id=user_id)
        return render_template("sell.html", symbols=[row["symbol"] for row in symbols_user])


@app.route("/password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")
        user_id = session["user_id"]

        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        if not check_password_hash(rows[0]["hash"], current_password):
            return apology("Wrong current password", 403)

        elif new_password != confirmation:
            return apology('Passwords are not the same', 403)

        elif current_password == new_password:
            return apology('The new password cannot be the same as the old password.', 403)

        hash = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, user_id)

        flash("Your password has been changed !")

        return redirect("/")

    else:
        return render_template("password.html")