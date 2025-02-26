import requests
import shutil
import os

url = "https://emoji.fileformat.info/png/1f600.png"


fontset = "Greek And Coptic"
# fontset = "Enclosed Alphanumeric Supplement"

if fontset == "Emoticons":
	start = 0x1f600
	end = 0x1f64F
elif fontset == "Enclosed Alphanumeric Supplement":
	start = 0x1F100
	end = 0x1F1FF
elif fontset == "General Punctuation":
	start = 0x2000
	end = 0x206F
elif fontset == "Greek And Coptic":
	start = 0x370
	end = 0x3FF

http_session = requests.session()
headers = {"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36"}

if os.path.exists(fontset) == False:
    os.mkdir(fontset)
if os.path.exists(fontset + "/25/") == False:
    os.mkdir(fontset + "/25/")
if os.path.exists(fontset + "/100/") == False:
    os.mkdir(fontset + "/100/")
if os.path.exists(fontset + "/25/") == False:
    os.mkdir(fontset + "/25/")

for i in range(start, end):
	file_name = format(i, "X").zfill(4)
	print(f"download \"{fontset}/100/{file_name}.png\"")
	# if fontset == "Emoticons":
	# 	url = "https://emoji.fileformat.info/png/" + format(i, "x") + ".png"
	# else:
	url = "https://www.fileformat.info/info/unicode/char/" + format(i, "x") + "/sample.png"
	response = http_session.get(url, headers=headers, stream=True)

	file_handle = open(fontset + "/100/" + file_name + ".png", "wb")

	shutil.copyfileobj(response.raw, file_handle)

	file_handle.close()
	del response

	os.system(f"convert \"{fontset}/100/{file_name}.png\" -crop 100x100+0+0 +repage -resize 25x25 \"{fontset}/25/{file_name}.png\"")
	print(f"convert \"{fontset}/100/{file_name}.png\" -crop 100x100+0+0 +repage -resize 25x25 \"{fontset}/25/{file_name}.png\"")

_fontset = fontset.replace(" ", "_").lower()
os.system(f"tex3ds -o \"{fontset}/{_fontset}_font.t3x\" -f a4 -z auto --atlas -p \"{fontset}\"/preview.png \"{fontset}\"/25/*")
print(f"tex3ds -o \"{fontset}/{_fontset}_font.t3x\" -f a4 -z auto --atlas -p \"{fontset}\"/preview.png \"{fontset}\"/25/*")


print("Done!")