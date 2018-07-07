import web
import io, os
import time
import datetime
import sqlite3
from PIL import Image

urls = (
    '/', 'Upload',
    '/list', 'List',
    '/reuse', 'Reuse'
)

render = web.template.render('templates', base='layout', globals={'datetime':datetime, 'time':time, 'os':os})

IMAGE_PATH = '/home/laiwd3/image_transport_ws/src/image_trans/src/roger.jpg'
CURRENT_IMAGE = 'static/current.jpg'

db = sqlite3.connect("server.db")
cur = db.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS upload_log(time bigint primary key, ip text, size int, filename text);")
db.commit()

def getcursor():
    db = sqlite3.connect("server.db")
    cur = db.cursor()
    return db, cur

class List:
    def GET(self):
        db, cur = getcursor()
        cur.execute("SELECT * FROM upload_log ORDER BY time DESC")
        return render.list(data = cur.fetchall())

class Reuse:
    def GET(self):
        db, cur = getcursor()
        fn = web.input()
        filename = os.path.basename(fn["filename"])
        data = Image.open("./static/uploads/" + filename)
        data = data.convert("RGB")
        data.save(IMAGE_PATH)
        data.save(CURRENT_IMAGE)
        raise web.seeother('/')

class Upload:
    def GET(self):
        db, cur = getcursor()
        cur.execute("SELECT * FROM upload_log ORDER BY time DESC LIMIT 9")
        return render.index(data = cur.fetchall())
    def POST(self):
        db, cur = getcursor()
        x = web.input(myfile={})
        bio = io.BytesIO(x['myfile'].value)
        data = Image.open(bio)
        ts = int(time.time() * 1000)
        data = data.convert("RGB")
        data.save("./static/uploads/%d.jpg" % ts)
        data.save(IMAGE_PATH)
        data.save(CURRENT_IMAGE)
        cur.execute("REPLACE INTO upload_log VALUES (?, ?, ?, ?)", (
            ts, 
            web.ctx["ip"], 
            len(x['myfile'].value), 
            "/static/uploads/%d.jpg" % ts
        ))
        db.commit()
        raise web.seeother('/')


if __name__ == "__main__":
   app = web.application(urls, globals()) 
   app.run()