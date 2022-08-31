import time
import os
from TimerManager import TimerManager, Task
from flask import Flask, request

app = Flask(__name__)
timer = None
db = []

def get_handler():
    return "<p>Hello, World!</p>"

def post_handler(request):
    req_data = request.json
    
    if "secs_from_now" not in req_data:
        return "ERROR"
    try:
        seconds = int(req_data["secs_from_now"])
    except:
        return "ERROR"
    
    new_task = Task(time.time() + seconds, 
                    os.system, f'osascript -e \'display notification "Task no. {len(db)}"\'')
    db.append(new_task)
    timer.add_task(new_task)
    return f"added task in {seconds} seconds from now"

def del_handler(request):
    return "DEL REQ"

@app.route("/", methods=['GET', 'POST', 'DELETE'])
def handler():
    lut = { "GET" : get_handler, 
            "POST": post_handler,
            "DELETE": del_handler }
    return lut[request.method](request)


if __name__ == "__main__":
    timer = TimerManager()
    timer.create_consumer_processes()
    app.run(host='0.0.0.0', port=81)

