from django.shortcuts import render
from django.db.models.functions import Extract
from django.core import serializers
from django.http.response import JsonResponse
from django.http import HttpResponse
from urllib import parse
from .models import Books
import json
from time import time, sleep

def filter(data, filters):
	data = parse.unquote(data).lstrip("/").upper()
	return any(f in data for f in filters)

def index(request):
	timeout = 3.0
	max_entries = 100
	sqli_filters = [";", "/", "\\", "*", "=", "#", "&", "%", "<", ">", "^", "|", "$", "0X", "+", "~", "!", "@"]
	sqli_filters += ["TRUE", "FALSE", "OR", "JOIN", "DIV", "LIKE", "REGEXP", "ST", "CONVERT", "IS", "NOT", "MATCH","BINARY", "BETWEEN", "ISNULL", "ALIAS", "ROUND", "POW", "LOAD", "VERSION", "LIMIT", "OFFSET", "ALL", "ANY", "EXISTS", "SOME", "IN"]

	if filter("?".join(request.GET.get('search_with', "")), sqli_filters):
		return HttpResponse(status=500)

	start = time()

	books = {'res': "list"}
	search_with, key = request.GET.get('search_with', None), request.GET.get('key', "")
	search_res = None

	if search_with == None:
		books['res'] = "entry"
		timeout = 0.0
	elif search_with == "title":
		search_res = Books.objects.filter(title__icontains=key)
	elif search_with == "author":
		search_res = Books.objects.filter(author__icontains=key)
	elif search_with == "publisher":
		search_res = Books.objects.filter(author__icontains=key)
	elif "year" in search_with or "month" in search_with:
		search_res = Books.objects.annotate(target=Extract('pubdate', search_with)).filter(target=key)
	elif search_with:
		books['res'] = "error"

	if search_res:
		books = {"books": [book for book in search_res if Books.isValidISBN(book.isbn)][:max_entries]}

	elapsed = time() - start
	if elapsed < timeout:
		sleep(timeout - elapsed)

	return render(request, 'index.html', books)

def isbnsearch(request, isbn):
	if Books.objects.filter(isbn=isbn).exists():
		book = Books.objects.filter(isbn=isbn)
		json_data = json.loads(serializers.serialize("jsonl", book))["fields"]
		json_data["pubdate"] = json_data["pubdate"].split("T")[0]
		return JsonResponse(json_data)

	return HttpResponse(status=404)
