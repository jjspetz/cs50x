from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from passlib.context import CryptContext

from helpers import *

# for encrypting of passwords
# https://pythonhosted.org/passlib/narr/quickstart.html
pwd_context = CryptContext(schemes=["pbkdf2_sha256", "des_crypt"], deprecated="auto")

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Displays Portfolio in a table"""
    table = db.execute("SELECT * FROM portfolio WHERE id = :user_id", user_id=session["user_id"])
    stocks = table[0]["stocks"].split(",")
    quantity = table[0]["quantity"].split(",")
    
    # handles cases where a stock is duplicted
    # if too hard can be handled in buy/sell and current buy can be used for history
        
    count = len(stocks)
    shareprice = []
    totalprice = []
    
    for i in range(count):
        shareprice.append(usd(find_shareprice(stocks[i])))
        totalprice.append(usd(find_purchaseprice(stocks[i], quantity[i])))
    
    return render_template("index.html", stocks=stocks, quantity=quantity, sp=shareprice, tp=totalprice, count=count, table=table)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("symbol") or not request.form.get("symbol").isalpha() :
            return apology("must provide a stock symbol")

        # ensure password was submitted
        elif not request.form.get("quantity") or not request.form.get("quantity").isdigit()::
            return apology("must enter the amount of stock to buy")
        
        # finds how much cash the current user has avialable
        cash_list = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])
        # convert returned list value to a float
        cash = cash_list[0].get("cash")
        
        purchase_price = find_purchaseprice(request.form.get("symbol"), request.form.get("quantity"))
        if purchase_price > cash:
            return apology("you don't have enough money")
        else:    
            # returns current stock and quantity string in portfolio
            portfolio = db.execute("SELECT * FROM portfolio WHERE id = :user_id", user_id = session["user_id"])
    #######
    #           
    #       
    #           
    #           
    ########
            if portfolio[0]["stocks"] != None:
                stocks = str(portfolio[0]["stocks"])
                stocks += "," + request.form.get("symbol").upper()
                quantity = str(portfolio[0]["quantity"])
                quantity += "," + request.form.get("quantity")
        
                # inserts stock into portfolio (needs fix currently writes over old buys build a sting array)
                db.execute('UPDATE portfolio SET stocks=:stocks, quantity=:quantity WHERE id=:user_id', stocks=stocks, quantity=quantity, user_id=session["user_id"])
            else:
                # inserts stock into portfolio (needs fix currently writes over old buys build a sting array)
                db.execute('UPDATE portfolio SET stocks=:stocks, quantity=:quantity WHERE id=:user_id', stocks=request.form.get("symbol"), quantity=request.form.get("quantity"), user_id=session["user_id"])
            
            # delete cash from users table
            cash -= purchase_price
            db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", cash=cash, user_id=session["user_id"])
            
            return redirect(url_for("index"))
            
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["pwd_hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol")
    
        quote = lookup(request.form.get("symbol"))
    
        if quote == None: 
            return apology("couldn't find {} stock".format(request.form.get("symbol")))
        else:
            return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("username"):
            return apology("must provide password")
            
        # ensure password conformation was submitted and it matches password
        elif request.form.get("password") != request.form.get("pass_con"):
            return apology("password and password conformation didn't match")

        # hash the password
        pwd_hash = pwd_context.hash(request.form.get("password"))
        
        
        # query database for username to make sure it isn't already taken
        result = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if not result:
        
            # Enters registration's information into the data base
            db.execute("INSERT INTO users (username, pwd_hash) VALUES(:username, :pwd_hash)", username=request.form.get("username"), pwd_hash=pwd_hash)
        
            # automatically logs in the new registration assuming all checks passed
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            
            # creates portfolio with same id as well
            db.execute("INSERT INTO portfolio (id) VALUES(:user_id)", user_id=session["user_id"])

            return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")