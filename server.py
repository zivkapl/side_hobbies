import time
import os
from TimerManager import TimerManager, Task
from flask import Flask, request

app = Flask(__name__)
timer = None
db = {}

def get_handler(request):
    return "Currently active timers: {}".format("".join([ 'task ' + str(uuid) + '\n' for uuid in db.keys() ]))

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
    db[str(new_task.get_uuid())] = new_task
    timer.add_task(new_task)
    return f"added task in {seconds} seconds from now"

def del_handler(request):
    req_data = request.json
    if "id" not in req_data:
        return "ERROR"

    try:
        timer.remove_task(db[req_data["id"]])
    except:
        return "no such task"
    return "removed task {}".format(req_data["id"])

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

