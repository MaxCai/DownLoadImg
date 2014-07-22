# -*- coding: utf-8 -*- 

#python3.2

import urllib.request as request
import urllib.error
import urllib.response
import re

def getHrefFromHome(url_home):
	req = request.Request(url_home)
	imgHref = []
	try:
		response = request.urlopen(req)
		page = response.read()
		pattern = re.compile(r'data-location="content"\s+href=".+?"', re.I)
		exp_it = re.finditer(pattern, str(page))
		url_pattern = re.compile(r'http:.+/')
		for match in exp_it:
			grp = match.group()
			print("match ", grp)
			url_match = url_pattern.search(str(grp))
			if url_match:
				imgHref.append(url_match.group())
	except HTTPError as e:
		print("http error: ", e.code)
	except URLError as e:
		print("url error: ", e.code)
	finally:
		print("get ", imgHref)
		return imgHref

img_pattern = re.compile(r'data-pic="http://photos.tuchong.com/.+/f/.+?jpg', re.I)
left_pattern = re.compile(r'"icon-batch-large batch-arrow-left" href=".+?"')
right_pattern = re.compile(r'"icon-batch-large batch-arrow-right" href=".+?"')
jpg_pattern = re.compile(r'http://.+?jpg')
href_pattern = re.compile(r'http://.+/\d+?/')

left_href = []
right_href = []
img_href = []

def getImgPage(href):
	req = request.Request(href)
	response = request.urlopen(req)
	imgPage = response.read()
	return str(imgPage)

def getImgHref(imgPage):
	match = img_pattern.search(imgPage)
	if match:
		jpg_match = jpg_pattern.search(match.group())
		if jpg_match:
			img_href.append(jpg_match.group())
	
def getLeftUrl(imgPage):
	match = left_pattern.search(imgPage)
	if match:
		left_match = href_pattern.search(match.group())
		if left_match:
			return left_match.group()
	return ' '

def getRightUrl(imgPage):
	match = right_pattern.search(imgPage)
	if match:
		right_match = href_pattern.search(match.group())
		if right_match:
			return right_match.group()
	return ' '
	
def downLoadImg():
	cnt = 0
	for href in img_href:
		req = request.Request(href)
		response = request.urlopen(req)
		img = response.read()
		file = open('tuchong_' + str(cnt) + '.jpg', 'wb')
		file.write(img)
		file.close()
		cnt = cnt + 1
		
def downLoadAllPixmap(url_home):
	imgHref  = getHrefFromHome(url_home)
	for href in imgHref:
		imgPage = getImgPage(href)
		getImgHref(imgPage)
		max_find = 5
		left = getLeftUrl(imgPage)
		for i in range(0, max_find):
			if left == ' ':
				break
			left_img_page = getImgPage(left)
			getImgHref(left_img_page)
			left = getLeftUrl(left_img_page)
			
		right = getRightUrl(imgPage)
		for i in range(0, max_find):
			if right == ' ':
				break
			right_img_page = getImgPage(right)
			getImgHref(right_img_page)
			right = getRightUrl(right_img_page)
		
	print('------------------------------')
	print("img href: ", img_href)
	downLoadImg()
		
	#print("left href: ", left_href)
	#print("right href: ", right_href)

def main():
	url_home = r'http://tuchong.com/'
	downLoadAllPixmap(url_home)
	
if __name__ == "__main__":
	main()