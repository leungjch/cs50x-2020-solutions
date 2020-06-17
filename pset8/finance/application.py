import os

from cs50 import SQL
import requests
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userid = session['user_id']
    trades = db.execute("SELECT * FROM current_holdings WHERE user_id = ?", userid)
    return render_template("home.html", rows=trades)
    # return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # check if requested stock exists

        # check how much money user has

        # number of shares to buy
        nbuy = request.form.get("nbuy")
        ticker = request.form.get("ticker")

        # lookup ticker
        info = lookup(ticker)
        userid = session['user_id']
        # get username
        cash = db.execute("SELECT cash FROM users WHERE id = ?", userid)[0]['cash']
        print(cash, userid, info, nbuy)
        # add trade and redirect to index (to show current portfolio)
        if info:
            expenses = float(nbuy)*float(info['price'])
            # check if user has enough cash
            if expenses > cash:
                return apology("insufficient funds, try again", 403)

            # add trade
            db.execute("INSERT INTO trades (user_id, symbol, type, n_shares, price) VALUES (?, ?, ?, ?, ?)", userid, info['symbol'], 'buy', nbuy, info['price'])

            # update user cash (deduct)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - expenses, userid)

            holding = db.execute("SELECT * FROM current_holdings WHERE user_id = ? AND symbol = ?", userid, info['symbol'])
            # update portfolio (check if user has bought stock before)
            if holding:
                # update stock count
                db.execute("UPDATE current_holdings SET n_shares = ? WHERE user_id = ? AND symbol = ?", int(holding[0]['n_shares'])+int(nbuy), userid, info['symbol'])
            else: # add a new entry
                db.execute("INSERT INTO current_holdings (user_id, symbol, n_shares, current_price) VALUES (?,?,?,?)", userid, info['symbol'], nbuy, info['price'])

            return redirect("/")
        # check if incorrect symbol
        else:
            return apology("invalid symbol", 403)

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userid = session['user_id']
    history = db.execute("SELECT * FROM trades WHERE user_id = ?", userid)
    return render_template("history.html", rows=history)
    # return apology("TODO")

    return apology("TODO")


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
        ticker = request.form.get("quote")
        info = lookup(ticker)
        if info:
            # if successful, return stock quote viewer
            return render_template("quoted.html", symbol = info['symbol'], name = info['name'], currentprice = info['price'])
        else:
            return apology("invalid symbol", 403)
    # Load page if GET request
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        print('hi')
        # check if info is correct
        if request.form.get("username") == '' or request.form.get("pass1") != request.form.get("pass2") or request.form.get("pass1") == '' or request.form.get("pass2") == '':
            return apology("Please check that the registration info is correct.")
        # submit info to database
        else:
            username = request.form.get("username")
            print("username is ", username)

            hashed = generate_password_hash(request.form.get("password1"))
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed)
            return render_template("login.html")
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # check if requested stock exists

        # check how much money user has

        # number of shares to buy
        nsell = request.form.get("nsell")
        ticker = request.form.get("ticker")

        # lookup ticker
        info = lookup(ticker)
        userid = session['user_id']
        # get username
        cash = db.execute("SELECT cash FROM users WHERE id = ?", userid)[0]['cash']
        holdings = db.execute("SELECT * FROM current_holdings WHERE user_id = ? AND symbol = ?", userid, ticker)
        # check if ticker exists
        if info:
            revenue = float(nsell)*float(info['price'])
            # check if user has the stock that they are trying to sell
            if holdings:
                # add trade
                db.execute("INSERT INTO trades (user_id, symbol, type, n_shares, price) VALUES (?, ?, ?, ?, ?)", userid, info['symbol'], 'sell', nsell, info['price'])

                # update user cash (deduct)
                db.execute("UPDATE users SET cash = ? WHERE id = ?", float(cash) + float(revenue), userid)

                # deduct shares from holdings (also check if user sold all stocks)
                if int(holdings[0]['n_shares']) == int(nsell):
                    db.execute("DELETE FROM current_holdings WHERE user_id = ? AND symbol = ?", userid, info['symbol'])
                else:
                    db.execute("UPDATE current_holdings SET n_shares = ? WHERE user_id = ? AND symbol = ?", int(holdings[0]['n_shares'])-int(nsell), userid, info['symbol'])

                # redirect to home
                return redirect("/")

        # check if incorrect symbol
        else:
            return apology("invalid symbol", 403)


    else:
        return render_template("sell.html")
    return apology("TODO")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
