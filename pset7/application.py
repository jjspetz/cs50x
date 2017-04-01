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
    # finds all the users stock information and formates it grouping similar stocks
    portfolio = db.execute("SELECT stocks, SUM(quantity) FROM transactions \
    WHERE id=:user_id GROUP BY stocks", user_id=session["user_id"])
    
    # finds the needed cash information from users
    user_info = db.execute("SELECT username, cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    
    # declares list to populate the index.html
    shareprice = []
    totalprice = []
    count = (len(portfolio))
    net_value = 0
    
    # puts the information into easy to interate over lists
    for i in range(count):
        shareprice.append(usd(find_shareprice(portfolio[i]["stocks"])))
        # tmp file to call purchase only once
        tmp = find_transactionprice(portfolio[i]["stocks"], portfolio[i]["SUM(quantity)"])
        net_value += tmp
        totalprice.append(usd(tmp))
        
    # finishes calculating net value
    net_value = net_value + user_info[0]['cash']
    
    # formates values into dollar form
    cash = usd(user_info[0]['cash'])
    net_value = usd(net_value)
    
    return render_template("index.html", sp=shareprice, tp=totalprice, count=count,\
    port=portfolio, cash=cash, user=user_info[0]["username"], netval=net_value)

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
        elif not request.form.get("quantity") or not request.form.get("quantity").isdigit():
            return apology("must enter the amount of stock to buy")
        
        elif float(request.form.get("quantity")) <= 0:
            return apology("enter a positive number")
        
        else:
            quote = lookup(request.form.get("symbol"))
             
            if quote == None: 
                return apology("couldn't find {} stock".format(request.form.get("symbol").upper()))
    
            else:
                # finds how much cash the current user has avialable
                cash_list = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])
                # convert returned list value to a float
                cash = float(cash_list[0].get("cash"))
        
                purchase_price = find_transactionprice(request.form.get("symbol"), request.form.get("quantity"))
                if purchase_price > cash:
                    return apology("you don't have enough money")
            
                else:
                    db.execute("INSERT INTO transactions (id, stocks, quantity, price) \
                    VALUES(:userid, :stocks, :quantity, :price)",
                    userid=session["user_id"], stocks=request.form.get("symbol").upper(), price=purchase_price,
                    quantity=request.form.get("quantity"))
    
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
    # gets the transaction SQL table and formates it for use here
    history = db.execute("SELECT stocks, quantity, price, timestamp FROM transactions WHERE id=:userid",
    userid=session["user_id"])
    
    # declares list to populate the history.html
    count = (len(history))
    price = []
    
    for i in range(count):
        price.append(usd(history[i]["price"]))
    
    return render_template("history.html", history=history, count=count, price=price)

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
            return apology("couldn't find {} stock".format(request.form.get("symbol").upper()))
        else:
            price = usd(quote["price"])
            return render_template("quoted.html", name=quote["name"], price=price, symbol=quote["symbol"])
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
            db.execute("INSERT INTO users (username, pwd_hash) VALUES(:username, :pwd_hash)",
            username=request.form.get("username"), pwd_hash=pwd_hash)
        
            # automatically logs in the new registration assuming all checks passed
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            
            # creates portfolio with same id as well
            #db.execute("INSERT INTO portfolio (id) VALUES(:user_id)", user_id=session["user_id"])

            return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
    
        # finds all the users stock information
        portfolio = db.execute("SELECT stocks, SUM(quantity) FROM transactions WHERE id=:user_id GROUP BY stocks",
        user_id=session["user_id"])
        
        # checks to make sure user entered a positive number for quantity
        quantity = request.form.get("quantity")
        if not quantity or not quantity.isdigit() or float(quantity) < 0:
            return apology("enter a positive number for quantity")
            
        switch = 2 #initialze switch variable
    
        # interate over all stocks in profile 
        for i in range(len(portfolio)):
            # looks for the user entered stock in the list
            if portfolio[i]["stocks"] == request.form.get("symbol").upper():
                switch = 0
                # handles case where user tries to sell more shares than the user owns
                if portfolio[i]["SUM(quantity)"] < int(request.form.get("quantity")):
                    switch = 1
        
        # the selected stock was owned and user sold some or all of owned shares
        if switch == 0:
            # declare variable for formating
            quantity = int(request.form.get("quantity"))
            quantity = -quantity
            
            # calculates sale price
            sale_price = find_transactionprice(request.form.get("symbol"), request.form.get("quantity"))
            
            # inserts the new transaction into the SQL table transaction
            db.execute("INSERT INTO transactions (id, stocks, quantity, price) \
            VALUES(:userid, :stocks, :quantity, :price)",
            userid=session["user_id"], stocks=request.form.get("symbol").upper(), quantity=quantity, price=sale_price)
            
            # finds current user's cash and calculates it
            user_info = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
            cash = user_info[0]['cash'] + sale_price
            
            # updates users new cash after sale
            db.execute("UPDATE users SET cash=:cash WHERE id=:userid", cash=cash, userid=session["user_id"])
            
        # prints apology for cas where user tries to sell more shares than the user owns
        elif switch == 1:
            return apology("You don't own that many shares of {}".format(request.form.get("symbol").upper()))
        
        # checks if user entered stock was in the profile, if not returns an apology
        else:   
            return apology("You don't own that stock!")
            
        return redirect(url_for("index"))
        
    else:
        return render_template("sell.html")

    
@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Allows user to add cash to their balance"""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # gets date from SQL table
        query = db.execute("SELECT cash FROM users WHERE id = :userid", userid = session["user_id"])
        
        # checks for proper input
        if request.form.get("cash").isdigit():
            cash = query[0]["cash"] + float(request.form.get("cash"))
        
            # puts cash into SQL table
            db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", cash=cash, user_id=session["user_id"])
        
            return redirect(url_for("index"))
        
        else:
            return apology("enter a number")
    else:
        return render_template("addcash.html")