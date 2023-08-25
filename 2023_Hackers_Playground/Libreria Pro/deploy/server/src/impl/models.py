from django.db import models

# Create your models here.
class Books(models.Model):
	title = models.CharField(max_length = 257)
	image = models.CharField(max_length = 257)
	author = models.CharField(max_length = 129)
	price = models.IntegerField(null=True, blank=True)
	publisher = models.CharField(max_length = 129)
	pubdate = models.DateTimeField(null=True, blank=True)
	isbn = models.CharField(max_length = 17)
	description = models.TextField(max_length = 4097)

	#10 or 13 digit integer
	@staticmethod
	def isValidISBN(isbn):
		return isbn == None or (str(int(isbn)) == isbn and (len(isbn) == 10 or len(isbn) == 13))

class t0p5ecr3t(models.Model):
	key = models.CharField(max_length=65)
	value = models.CharField(max_length=129)
