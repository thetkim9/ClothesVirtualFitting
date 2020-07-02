from flask import Flask, request, send_file
from flask_limiter import Limiter
from PIL import Image, ImageOps
import PIL
from ga import track_event
from subprocess import Popen, PIPE
import os

app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 1024 * 1024 * 8

limiter = Limiter(app, default_limits=['1 per second'])

@app.route('/fit', methods=['POST'])
def fit():
  track_event(category='ClothesFit', action='fit')

  if not request.files.get('person_image'):
    return {'error': 'must have a person image'}, 300

  if not request.files.get('shirt_image'):
    return {'error': 'must have a shirt image'}, 300

  try:
    print("hi0")
    person_image = Image.open(request.files['person_image'].stream)
    shirt_image = Image.open(request.files['shirt_image'].stream)
    if(person_image.format not in ['JPG', 'JPEG', 'PNG']):
      return {'error': 'image must be jpg, jpeg or png'}, 500

    if(shirt_image.format not in ['JPG', 'JPEG', 'PNG']):
      return {'error': 'image must be jpg, jpeg or png'}, 500

    #print("hi1", person_image, shirt_image)
    dir1 = "images/person."+person_image.format.lower()
    person_image.save(dir1)
    #print("hi2")
    dir2 = "images/t-shirt."+shirt_image.format.lower()
    shirt_image.save(dir2)
    #print("hi3")
    p = Popen(['./a.out'], shell=True, stdout=PIPE, stdin=PIPE)
    value = (dir1 + '\n').encode('UTF-8')  # Needed in Python 3.
    p.stdin.write(value)
    p.stdin.flush()
    value = (dir2 + '\n').encode('UTF-8')  # Needed in Python 3.
    p.stdin.write(value)
    p.stdin.flush()
    p.wait()
    #print("hi4")

    msg, err = p.communicate()

    if msg!=None and len(msg)>0:
        return {'error': 'face not properly recognized. choose a photo with an upfront person.'}, 600

    #print("hi5.5")
    result = send_file("./images/final.jpg", mimetype='image/jpg')

    #print("hi6")
    return result

  except Exception:
    return {'error': 'can not load your image files. check your image files'}, 800

@app.errorhandler(413)
def request_entity_too_large(error):
  return {'error': 'File Too Large'}, 413

if __name__ == '__main__':
  app.run(debug=False, port=80, host='0.0.0.0')
