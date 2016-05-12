from flask_assets import Bundle

bundles = {

    'js': Bundle(
        # 'static/js/jquery-1.10.2.js',
        'js/material.min.js',
        # 'js/crudMonitores.js'
    ),

    'css': Bundle(
        'css/material.min.css',
        'css/style.css'
    )
}
