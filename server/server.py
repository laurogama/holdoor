# coding=utf-8
import datetime
import logging

from flask import render_template, request, jsonify, flash, url_for, redirect
from flask.ext.assets import Environment

from ErrorHandler import InvalidUsage
from Models import Button, Product, Cracha, User, Location
from assets import bundles
from bootstrap import app, db
from settings import CONFIG

assets = Environment(app)
assets.register(bundles)
configuration = CONFIG
app.config['SECRET_KEY'] = configuration['SECRET_KEY']
app.config['SQLALCHEMY_DATABASE_URI'] = configuration['SQLITE_TEST_DB']


@app.route('/')
def index():
    buttons = Button.query.all()
    products = Product.query.all()
    crachas = Cracha.query.all()
    return render_template("index.html", buttons=buttons, products=products, crachas=crachas)


@app.route('/user/')
def user_list():
    users = User.query.all()
    return render_template('user/user_list.html', users=users)


@app.route('/user/new/', methods=['POST', 'GET'])
def user_new():
    try:
        if request.method == 'POST':
            user = User(request.form['username'], request.form['rfid'])
            db.session.add(user)
            db.session.commit()
            flash('User was added successfully.')
        return render_template('user/user_new.html')
    except Exception as exception:
        print exception


@app.route('/user/delete/<user_id>', methods=['POST', 'GET'])
def user_delete(user_id):
    user = User.query.filter_by(id=user_id).first()
    db.session.delete(user)
    db.session.commit()
    return redirect(url_for('user_list'))
    pass


@app.route('/locations/')
def location_list():
    locations = Location.query.all()
    return render_template('location/location_list.html', locations=locations)


def validate_form(form):
    for item in form:
        # print "{}:{}->{}".format(item, form[item], type(form[item]))
        if not form[item]:
            flash('fields cant be empty', 'error')
            return False
    return True


@app.route('/locations/new/', methods=['POST', 'GET'])
def location_new():
    if request.method == 'GET':
        return render_template('location/location_new.html')
    elif request.method == 'POST' and validate_form(request.form):
        location = Location(request.form['mac'], request.form['name'], request.form['longitude'],
                            request.form['latitude'], request.form['defcon'])
        location.add()
        flash('Location added')
    else:
        flash("Invalid form.")
    return render_template('location/location_new.html')


@app.route('/button/')
def button_list():
    buttons = Button.query.all()
    return render_template("button/button_list.html", buttons=buttons)


@app.route('/button/reset/<button_id>')
def button_reset_purchases(button_id):
    try:
        if button_id:
            button = Button.query.filter_by(id=button_id).first()
            if button is not None:
                button.reset_counter()
                db.session.commit()
                flash('Purchases counter reset.')
        return redirect(url_for('index'))

    except Exception as exception:
        print exception
        flash('Error reseting counter')
        return redirect(url_for('index'))
    pass


@app.route('/product/')
def product_list():
    try:
        products = Product.query.all()
        return render_template("product/product_list.html", products=products)
    except Exception as exception:
        print exception
        raise InvalidUsage('Still coding', status_code=500)


@app.route('/product/new/', methods=['GET', 'POST'])
def product_new():
    try:
        if request.method == 'POST':
            product = Product(request.form['name'], request.form['description'])
            db.session.add(product)
            db.session.commit()
            flash('Product was added successfully.')
        return render_template('product/product_new.html')
    except Exception as exception:
        print exception


@app.route('/cracha_list/')
def cracha_list():
    try:
        crachas = Cracha.query.all()
        return render_template("cracha/cracha_list.html", crachas=crachas)
    except Exception as exception:
        print exception


@app.route('/click/', methods=['POST'])
def button():
    try:
        print request
        data = request.get_json(force=True)
        print data['mac']
        button = Button.query.filter_by(mac=data['mac']).first()
        if button is None:
            button = Button(data['mac'])
            db.session.add(button)
            db.session.commit()
        button.raise_counter()
        db.session.commit()
        print button
        logging.info("{}:{}".format(datetime.datetime.now(), button))
        return button.to_json(), 200

    except Exception as exception:
        print exception
        raise InvalidUsage('Still coding', status_code=500)


@app.route('/cracha/', methods=['POST'])
def cracha():
    try:
        print request
        print request.data
        data = request.get_json(force=True)
        print data['mac'], data['rfid']
        cracha = Cracha(data['mac'], data['rfid'])
        db.session.add(cracha)
        db.session.commit()
        logging.info("{}:{}".format(datetime.datetime.now(), cracha))
        if User.query.filter_by(rfid=cracha.rfid).first():
            return 'ok', 200
        else:
            return 'Not authorized', 401

    except Exception as exception:
        print exception
        raise InvalidUsage('Still coding', status_code=500)


@app.errorhandler(InvalidUsage)
def handle_invalid_usage(error):
    try:
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response
    except Exception as exception:
        print exception


if __name__ == '__main__':
    logging.basicConfig(filename=configuration['LOG_PATH'], level=logging.INFO, debug=True)
    db.create_all()
    try:
        app.run(host='0.0.0.0', port=configuration['APP_PORT'], debug=True)
    except Exception as exception:
        print exception
