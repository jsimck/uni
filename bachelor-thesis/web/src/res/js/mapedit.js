(function ($) {
    'use strict';

    var MapEdit = function () {
        this.$body = $('body');

        this.setUserOS();
        this.setUserAgent();
    };

    MapEdit.prototype.setUserOS = function () {
        var OSName = '';

        if (navigator.appVersion.indexOf('Win') != -1) OSName = 'windows';
        if (navigator.appVersion.indexOf('Mac') != -1) OSName = 'mac';
        if (navigator.appVersion.indexOf('X11') != -1) OSName = 'unix';
        if (navigator.appVersion.indexOf('Linux') != -1) OSName = 'linux';

        this.$body.addClass(OSName);
    };

    MapEdit.prototype.setUserAgent = function () {
        if (navigator.userAgent.match(/Firefox/i)) {
            this.$body.addClass('firefox');
        }
        if (navigator.userAgent.match(/Chrome/i)) {
            this.$body.addClass('chrome');
        }

        if (navigator.userAgent.match(/Android|BlackBerry|iPhone|iPad|iPod|Opera Mini|IEMobile/i)) {
            this.$body.addClass('mobile');
        } else {
            this.$body.addClass('desktop');
            if (navigator.userAgent.match(/MSIE 9.0/)) {
                this.$body.addClass('ie9');
            }
        }
    };

    MapEdit.prototype.toggleFullScreen = function (element) {
        // Supports most browsers and their versions.
        var requestMethod = element.requestFullScreen || element.webkitRequestFullScreen || element.mozRequestFullScreen || element.msRequestFullscreen;

        if (requestMethod) { // Native full screen.
            requestMethod.call(element);
        } else if (typeof window.ActiveXObject !== 'undefined') { // Older IE.
            var wscript = new ActiveXObject('WScript.Shell');
            if (wscript !== null) {
                wscript.SendKeys('{F11}');
            }
        }
    };

    MapEdit.prototype.initSlidingTabs = function () {
        $('a[data-toggle="tab"]').on('show.bs.tab', function (e) {
            e = $(e.target).parent().find('a[data-toggle=tab]');
            var hrefCurrent = e.attr('href');

            if (!$(hrefCurrent).is('.slide-left, .slide-right')) {
                return;
            }

            $(hrefCurrent).addClass('sliding');
            setTimeout(function () {
                $(hrefCurrent).removeClass('sliding');
            }, 100);
        });
    };

    MapEdit.prototype.initFormGroupDefault = function () {
        $('.form-group.form-group-default').click(function () {
            $(this).find('input').focus();
        });
        $('body').on('focus', '.form-group.form-group-default :input', function () {
            $('.form-group.form-group-default').removeClass('focused');
            $(this).parents('.form-group').addClass('focused');
        });

        $('body').on('blur', '.form-group.form-group-default :input', function () {
            $(this).parents('.form-group').removeClass('focused');
            if ($(this).val()) {
                $(this).closest('.form-group').find('label').addClass('fade');
            } else {
                $(this).closest('.form-group').find('label').removeClass('fade');
            }
        });

        $('.form-group.form-group-default .checkbox, .form-group.form-group-default .radio').hover(function () {
            $(this).parents('.form-group').addClass('focused');
        }, function () {
            $(this).parents('.form-group').removeClass('focused');
        });
    };

    MapEdit.prototype.initProgressBars = function () {
        $(window).on('load', function () {
            // Hack: FF doesn't play SVG animations set as background-image
            $('.progress-bar-indeterminate, .progress-circle-indeterminate, .mapplic-pin').hide().show(0);
        });
    };

    MapEdit.prototype.initUnveilPlugin = function() {
        // lazy load retina images
        $.fn.unveil && $('img[data-src]').unveil();
    };

    MapEdit.prototype.initSelectFxPlugin = function() {
        window.SelectFx && $('select[data-init-plugin="cs-select"]').each(function() {
            var el = $(this).get(0);
            $(el).wrap('<div class="cs-wrapper"></div>');
            new SelectFx(el);
        });
    };

    MapEdit.prototype.initPortlet = function () {
        $.fn.portlet && jQuery('[data-init-plugin="portlet"]').portlet();
    };

    MapEdit.prototype.init = function () {
        this.initFormGroupDefault();
        this.initSlidingTabs();
        this.initProgressBars();
        this.initSelectFxPlugin();
        this.initUnveilPlugin();
        this.initPortlet();
    };

    $.MapEdit = new MapEdit();
    $.MapEdit.constructor = MapEdit;

})(window.jQuery);

// Init on document ready
jQuery(document).ready(function ($) {
    // Init Dashnex
    $.MapEdit.init();
});