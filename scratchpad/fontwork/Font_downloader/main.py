import requests
import shutil
import os

out_put_path = input("Enter output path e.g. out/\n")
start_pos = input("Enter start unicode ID in hex. e.g. 4E00\n")
amount = input("Enter amount to download in hex. e.g. 3E\n")
start_pos = int(start_pos, 16)
amount = int(amount, 16)
print("Downloading from : " + hex(start_pos) + " to " + hex(start_pos + amount - 1))

if os.path.exists(out_put_path) == False:
    os.mkdir(out_put_path)

http_session = requests.session()

for i in range(amount):
    headers = {"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36", "referer" : "https://www.fileformat.info/info/unicode/char/" + format(start_pos + i, "x") + "/index.htm"}

    if i % 64 == 0:
        print("Current location : " + hex(start_pos + i))

    response = http_session.get("https://www.fileformat.info/info/unicode/char/" + format(start_pos + i, "x") + "/sample.png", headers=headers, stream=True)

    file_name = format(start_pos + i, "X").zfill(4)

    file_handle = open(out_put_path + file_name + ".png", "wb")

    shutil.copyfileobj(response.raw, file_handle)

    file_handle.close()
    del response

input("Done!!!!!\nPress enter to exit.\n")
