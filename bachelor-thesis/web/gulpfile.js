// Gulp plugins
var gulp = require('gulp'),
    filter = require('gulp-filter'),
    concat = require('gulp-concat'),
    uglify = require('gulp-uglify'),
    debug = require('gulp-debug'),
    merge = require('merge-stream'),
    flatten = require('gulp-flatten'),
    streamify = require('gulp-streamify'),
    imagemin = require('gulp-imagemin'),
    prefixer = require('gulp-autoprefixer'),
    cssnano = require('gulp-cssnano'),
    plumber = require('gulp-plumber'),
    sass = require('gulp-sass'),
    gif = require('gulp-if'),
    util = require('gulp-util');

var browserify = require('browserify'),
    babelify = require('babelify'),
    del = require('del'),
    watchify = require('watchify'),
    source = require('vinyl-source-stream'),
    browsersync = require('browser-sync').create();

// Define default destination folder
var res = 'src/res/';
var resBower = 'bower_components/';
var resNpm = 'node_modules/';
var dest = 'src/build/static/';

// Global options for plugins
var options = {
    debug: {
        minify: true,
        browsersync: false
    },
    browsersync: {
        proxy: 'localhost:8000',
        open: false
    },
    imagemin: {
        progressive: true,
        optimizationLevel: 3,
        interlaced: true
    },
    sass: {
        outputStyle: 'expanded'
    },
    cssnano: {
        postcssZindex: false,
        reduceIdents: false
    },
    prefixer: {
        browsers: ['> 1%', 'last 2 versions', 'Firefox ESR', 'ie >= 9'],
        cascade: false
    }
};

// Cleans all assets folders, useful for debugging
gulp.task('clean', function () {
    return del([
        dest + 'js/',
        dest + 'images/',
        dest + 'fonts/',
        dest + 'css/'
    ]);
});

// Vendor Task
gulp.task('vendor', function () {
    // Filters
    var jsFilter = filter('**/*.js', {restore: true});
    var cssFilter = filter('**/*.css', {restore: true});
    var fontFilter = filter(['**/*.eot', '**/*.svg', '**/*.ttf', '**/*.wof', '**/*.woff', '**/*.woff2'], {restore: true});
    var imageFilter = filter(['**/*.png', '**/*.gif', '**/*.jpg', '**/*.jpeg', '**/*.swf'], {restore: true});

    var vendorFiles = [
        // jQuery
        resBower + 'jquery/dist/jquery.js',
        // Bootstrap
        resBower + 'bootstrap/dist/css/bootstrap.css',
        resBower + 'bootstrap/dist/js/bootstrap.js',
        resBower + 'bootstrap/dist/fonts/glyphicons-halflings-regular.eot',
        resBower + 'bootstrap/dist/fonts/glyphicons-halflings-regular.svg',
        resBower + 'bootstrap/dist/fonts/glyphicons-halflings-regular.ttf',
        resBower + 'bootstrap/dist/fonts/glyphicons-halflings-regular.wof',
        // Font Awesome
        resBower + 'font-awesome/css/font-awesome.css',
        resBower + 'font-awesome/fonts/fontawesome-webfont.eot',
        resBower + 'font-awesome/fonts/fontawesome-webfont.svg',
        resBower + 'font-awesome/fonts/fontawesome-webfont.ttf',
        resBower + 'font-awesome/fonts/fontawesome-webfont.woff',
        resBower + 'font-awesome/fonts/fontawesome-webfont.woff2',
        // jQuery Actual
        resBower + 'jquery.actual/jquery.actual.js',
        // jQuery Unveil
        resBower + 'jquery.actual/jquery.actual.js',
        // jQuery Easing
        resBower + 'jquery.easing/js/jquery.easing.js',
        // jQuery Bez
        resBower + 'jquery-bez/jquery.bez.min.js',
        // React Datepicker
        resNpm + 'react-datepicker/dist/react-datepicker.css',
        // Custom MapEdit JS Plugins
        res + 'plugins/mapedit/*.*',
        // Custom Select FX plugin
        res + 'plugins/select-fx/*.*'
    ];

    return gulp
        .src(vendorFiles)
        .pipe(jsFilter)
        .pipe(concat('vendor.js', {newLine: ';'}))
        .pipe(gif(options.debug.minify, uglify()))
        .on('error', function (error) {
            util.log(util.colors.red(error));
        })
        .pipe(gulp.dest(dest + 'js/'))
        .pipe(jsFilter.restore)
        .pipe(cssFilter)
        .pipe(concat('vendor.css'))
        .pipe(gif(options.debug.minify, cssnano(options.cssnano)))
        .pipe(gulp.dest(dest + 'css/'))
        .pipe(cssFilter.restore)
        .pipe(fontFilter)
        .pipe(flatten())
        .pipe(gulp.dest(dest + 'fonts/'))
        .pipe(fontFilter.restore)
        .pipe(imageFilter)
        .pipe(flatten())
        .pipe(gif(options.debug.minify, imagemin(options.imagemin)))
        .pipe(gulp.dest(dest + 'images/'));
});

// Copies all image && font assets
gulp.task('copy-assets', function () {
    var imageFilter = filter(['**/*.png', '**/*.gif', '**/*.jpg', '**/*.jpeg', '**/*.swf'], {restore: true});

    var images = gulp
        .src(res + 'images/**')
        .pipe(imageFilter)
        .pipe(gif(options.debug.minify, imagemin(options.imagemin)))
        .pipe(imageFilter.restore)
        .pipe(gulp.dest(dest + 'images/'));

    var cssHacks = gulp
        .src([
            res + 'plugins/ie9.css',
            res + 'plugins/windows.chrome.fix.css'
        ])
        .pipe(cssnano(options.cssnano))
        .pipe(gulp.dest(dest + 'css'));

    var fonts = gulp
        .src(res + 'fonts/**')
        .pipe(gulp.dest(dest + 'fonts/'));

    return merge(images, cssHacks, fonts);
});

// Task handling sass compilation
gulp.task('sass', function () {
    var noPartials = function (file) {
        var path = require('path');
        var dirSeparator = path.sep.replace('\\', '\\\\');
        var relativePath = path.relative(process.cwd(), file.path);
        return !new RegExp('(^|' + dirSeparator + ')_').test(relativePath);
    };

    return gulp
        .src(res + 'scss/**/*.scss')
        .pipe(plumber())
        .pipe(filter(noPartials))
        .pipe(sass(options.sass))
        .on('error', sass.logError)
        .pipe(flatten())
        .pipe(gif(options.debug.minify, prefixer(options.prefixer)))
        .pipe(gif(options.debug.minify, cssnano(options.cssnano)))
        .pipe(gulp.dest(dest + 'css/'))
        .pipe(gif(options.debug.browsersync, browsersync.stream()));
});

// Watch task for sass
gulp.task('sass:watch', function () {
    gulp.watch(res + 'scss/**/*.scss', ['sass']);
});

// Build single js files
gulp.task('js', function() {
    return gulp
        .src(res + 'js/*.js')
        .on('error', function (error) {
            util.log(util.colors.red(error));
        })
        .pipe(gif(options.debug.minify, uglify()))
        .on('error', function (error) {
            util.log(util.colors.red(error));
        })
        .pipe(gulp.dest(dest  + 'js/'))
        .pipe(gif(options.debug.browsersync, browsersync.stream()));
});

// Watch task for single JS files build
gulp.task('js:watch', function () {
    gulp.watch(res + 'js/*.js', ['js']);
});

// React app build function
var buildReact = function (watch) {
    watch = (typeof watch !== 'undefined');

    var bundler = browserify({
        entries: './src/res/react/index.js',
        cache: {},
        packageCache: {},
        fullPaths: true
    }).transform('babelify', {presets: ['stage-2', 'react', 'es2015']});

    if (watch) {
        var watcher = watchify(bundler);
        return watcher
            .on('update', function () {
                var updateStart = Date.now();
                watcher.bundle()
                    .on('error', function (error) {
                        util.log(util.colors.red(error));
                    })
                    .pipe(source('mapedit.react.js'))
                    .pipe(gif(options.debug.minify, streamify(uglify())))
                    .pipe(gulp.dest(dest + 'js/'))
                    .pipe(gif(options.debug.browsersync, browsersync.stream()));
                util.log('React [' + util.colors.cyan('mapedit.react.js') + '] updated: ', util.colors.magenta((Date.now() - updateStart) + 'ms'));
            })
            .bundle()
            .on('error', function (error) {
                util.log(util.colors.red(error));
            })
            .pipe(source('mapedit.react.js'))
            .pipe(gif(options.debug.minify, streamify(uglify())))
            .pipe(gulp.dest(dest + 'js/'));
    } else {
        return bundler
            .bundle()
            .on('error', function (error) {
                util.log(util.colors.red(error));
            })
            .pipe(source('mapedit.react.js'))
            .pipe(gif(options.debug.minify, streamify(uglify())))
            .pipe(gulp.dest(dest + 'js/'));
    }
};

// Build react files
gulp.task('react', function () { return buildReact(); });
gulp.task('react:watch', function () { return buildReact(true); });

// Tasks purely to set debug variable
gulp.task('debug:true', function () {
    options.debug.minify = false;
    options.debug.browsersync = true;
    browsersync.init(options.browsersync);
});
gulp.task('debug:false', function () {
    options.debug.minify = false;
    options.debug.browsersync = false;
});

// Watch Assets
gulp.task('watch', ['debug:true', 'sass:watch', 'js:watch', 'react:watch']);

// Builds whole application from scratch
gulp.task('build', ['debug:false', 'vendor', 'copy-assets', 'sass', 'js', 'react']);