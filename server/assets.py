from flask_assets import Bundle

bundles = {

    'js': Bundle(
        # 'static/js/jquery-1.10.2.js',
        'js/jquery-2.2.3.min.js',
        'js/material.min.js',
        'js/datatables.min.js',

        # 'js/dataTables.responsive.min.js'
        # 'js/crudMonitores.js'
    ),

    'css': Bundle(
        'css/material.min.css',
        'css/style.css',
        'css/datatables.min.css',
        # 'css/responsive.dataTables.min.css'

    )
}
