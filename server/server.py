import datetime
import logging

from flask import render_template, request, jsonify, flash, url_for, redirect
from flask.ext.assets import Environment

from ErrorHandler import InvalidUsage
from Models import Button, Product
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
    return render_template("index.html", buttons=buttons, products=products)


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
