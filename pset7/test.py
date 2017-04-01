mylist = ['a', 'b', 'c', 'a', 'd', 'e', 'a', 'c']

table = [{'id': 1, 'quantity': '10,5,1,2,1,-10', 'stocks': 'spy,avt,GOOGL,jpm,GOOGL,spy'}]
stocks = table[0]["stocks"].split(",")
quantity = table[0]["quantity"].split(",")
total = 0

for i, x in enumerate(stocks):
    if stocks.count(x) > 1:
        total += int(quantity[i])

print (total)