import React, { Component, StyleSheet, View, WebView, TouchableOpacity, LayoutAnimation, Text, Dimensions, Platform, ProgressBarAndroid, ActivityIndicatorIOS } from 'react-native'
import Icon from 'react-native-vector-icons/EvilIcons'
import _ from 'lodash'
import tapEventPlugin from '../assets/TapEventPlugin'
import animatePlugin from '../assets/AnimatePlugin'

// Inject tap plugin on IOS
const injectScript = ((Platform.OS === 'ios') ? tapEventPlugin : '') + `
    (function() {
        var elms = document.querySelectorAll('g');
        for (var i = 0; i < elms.length; i++) {
            elms[i].addEventListener('${(Platform.OS === 'ios') ? 'tap' : 'click'}', function (e) {
                var actionId = this.getAttribute('id');
                document.location.hash = ((Math.floor((Math.random() * 10000) + 1)).toString() + '___' + actionId).toString();
            }, false);
        }
    })();
`;

class MapView extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            expanded: false,
            viewHeight: this.getShrinkedHeight()
        };

        // Actions cache
        this.stateChanged = false;
        this.actionIdsCounter = 0;
        this.actions = {};
    }

    parseAttributes(attr) {
        var parsedAttr = '';

        // Convert properties to string
        for (var prop in attr) {
            if (attr.hasOwnProperty(prop)) {
                parsedAttr += `${prop}="${attr[prop]}" `;
            }
        }

        // Remove last empty space
        return parsedAttr.substr(0, parsedAttr.length - 1);
    }

    getPosition(x, y) {
        return `
            <g class="animated bounceIn">
                <circle cx="${x}" cy="${y}" r="20" fill="red" />
                <circle cx="${x}" cy="${y}" r="15" fill="white" />
                <circle cx="${x}" cy="${y}" r="8" fill="red" />
            </g>
        `;
    }

    parseStyles(styles) {
        var parsedStyles = 'style="';
        var property = '';

        // Convert properties to string
        for (var prop in styles) {
            if (styles.hasOwnProperty(prop)) {
                // Replace came case props to dash-case
                property = prop.replace(/\.?([A-Z]+)/g, function (x,y) {
                    return '-' + y.toLowerCase()
                }).replace(/^_/, '');

                // Add webkit prefix
                if (prop.match(/transform/)) {
                    parsedStyles += `-webkit-${property}: ${styles[prop]}; `;
                } else {
                    parsedStyles += `${property}: ${styles[prop]}; `;
                }
            }
        }

        // Remove last empty space
        return parsedStyles.substr(0, parsedStyles.length - 1) + "\"";
    }

    parseAction(action) {
        // Check if action exist
        if (!action.hasOwnProperty('content')) {
            return '';
        }

        // Add action to actions array and increase ID counter
        this.actions[this.actionIdsCounter] = action;
        this.actionIdsCounter++;

        return `id="${this.actionIdsCounter - 1}"`;
    }

    parseTextContent(text, attr) {
        return _.map(text.split('\n'), (entry, index) => {
            return `<tspan x=${attr.x} dy="1.2em">${entry}</tspan>`;
        })
    }

    parseSvgElement(svg) {
        var parsedSvg = '';

        // Get default styles
        switch (svg.type) {
            case 'rect':
                parsedSvg = `<g ${this.parseAction(svg.action)}><rect ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)} /></g>`;
                break;

            case 'text':
                parsedSvg = `<g ${this.parseAction(svg.action)}><text ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)}>${this.parseTextContent(svg.data.content, svg.attr)}</text></g>`;
                break;

            case 'circle':
                parsedSvg = `<g ${this.parseAction(svg.action)}><circle ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)} /></g>`;
                break;

            case 'ellipse':
                parsedSvg = `<g ${this.parseAction(svg.action)}><ellipse ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)} /></g>`;
                break;

            case 'line':
                parsedSvg = `<g ${this.parseAction(svg.action)}><line ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)} /></g>`;
                break;

            case 'path':
                parsedSvg = `<g ${this.parseAction(svg.action)}><path ${this.parseStyles(svg.style)} ${this.parseAttributes(svg.attr)} /></g>`;
                break;
        }

        return parsedSvg;
    }

    getParsedSVG(source) {
        if (source == null) {
            return '';
        }

        // Parse data to svg string
        var svgStr = _.map(source.svg, svg => {
            return this.parseSvgElement(svg);
        }).join('');

        // Check if current user position should be rendered
        if (!_.isUndefined(this.props.position)) {
            svgStr += this.getPosition(this.props.position.x, this.props.position.y);
        }

        return svgStr;
    }

    getMapHtml(source) {
        return `<!doctype html><html style="background: #fafafa;" lang="en"><head><meta charset="UTF-8"><title>Map</title>${animatePlugin}</head><body style="padding: 40px; background: #fafafa; font-family: sans-serif;"><svg style="background: #fafafa;" width="1088" height="600" xmlns="http://www.w3.org/2000/svg">${this.getParsedSVG(source)}</svg></body></html>`;
    }

    getMaximizedHeight() {
        // We substract 64 (status bar + navigation bar) and 34 for padding
        return Dimensions.get('window').height - 64 - 27 - 34;
    }

    getShrinkedHeight() {
        var processedHeight = Dimensions.get('window').height - 64 - 27;

        // On smaller screens we expand map to 2/3 of the screen
        if ((Math.round(processedHeight) / 2) < 200) {
            return Math.round(processedHeight * 2/3);
        }

        // In shrinked state, the map view takes up 1/2 of the screen
        return Math.round(processedHeight / 2);
    }

    handleOnExpandPress() {
        // Animate view expand
        LayoutAnimation.configureNext(LayoutAnimation.Presets.easeInEaseOut);

        // Toggle webView size
        this.setState({
            expanded: !this.state.expanded,
            viewHeight: !this.state.expanded ? this.getMaximizedHeight() : this.getShrinkedHeight()
        });

        // Call callback function with current expand state
        this.props.onExpand(this.state.expanded);
    }

    handleOnStateChanged(navState) {
        // Extract actionId from url
        var parts = navState.url.split('___');
        var id = parseInt(parts[parts.length - 1]);

        // Check if action is valid number and call callback
        if (_.isNumber(id)) {
            // Reset state change to false after 1sec, that means we are blocking action
            // clicks for 500 ms, this prevents android from pushing view twice
            // This is needed only for android, ios doesn't fire too many state changes events for some reason
            if (Platform.OS == 'android' && !this.stateChanged) {
                this.stateChanged = true;

                setTimeout(() => {
                    this.stateChanged = false;
                }, 500);

                // call callback
                this.props.onActionClick(this.actions[parseInt(id)]);
            } else if (Platform.OS == 'ios') {
                // Just call callback with action
                this.props.onActionClick(this.actions[parseInt(id)]);
            }
        }
    }

    renderLoader() {
        return (
            <View style={[this.props.style, styles.container, {height: this.state.viewHeight}]}>
                {Platform.OS == 'ios' &&
                    <ActivityIndicatorIOS
                        style={styles.loadingIndicator}
                        animating={true}
                        color="#21bfbd"
                        size="large"
                    />
                }
                {Platform.OS == 'android' &&
                    <ProgressBarAndroid
                        style={styles.loadingIndicator}
                        color="#21bfbd"
                        indeterminate={true}
                    />
                }
            </View>
        );
    }

    render() {
        return (
            <View style={styles.parentContainer}>
                {_.isUndefined(this.props.source) ? this.renderLoader() :
                    <WebView
                        style={[this.props.style, styles.container, {height: this.state.viewHeight}]}
                        bounces={false}
                        scalesPageToFit={true}
                        automaticallyAdjustContentInsets={true}
                        onNavigationStateChange={(navState) => this.handleOnStateChanged(navState)}
                        javaScriptEnabled={true}
                        injectedJavaScript={injectScript}
                        source={{
                            html: this.getMapHtml(this.props.source)
                        }}
                    />
                }
                <TouchableOpacity onPress={() => this.handleOnExpandPress()} style={styles.expandButtonContainer}>
                    {!this.state.expanded && <Icon style={styles.expandButton} name="chevron-down" size={20} />}
                    {this.state.expanded && <Icon style={styles.expandButton} name="chevron-up" size={20} />}
                </TouchableOpacity>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        height: 300,
        alignSelf: 'stretch',
        alignItems: 'center',
        flexDirection: 'row',
        backgroundColor: '#fafafa',
        padding: 0,
        margin: 0
    },
    loadingIndicator: {
        flex: 1
    },
    parentContainer: {
        backgroundColor: '#fafafa'
    },
    expandButtonContainer: {
        flex: 1,
        height: 20,
        flexDirection: 'row',
        backgroundColor: '#21bfbd',
        justifyContent: 'center',
        alignItems: 'center'
    },
    expandButton: {
        textAlign: 'center',
        color: '#fff'
    },
    webView: {
        padding: 0,
        margin: 0
    }
});

MapView.propTypes = {
    style: React.PropTypes.number,
    source: React.PropTypes.object,
    position: React.PropTypes.object,

    // Functions
    onExpand: React.PropTypes.func,
    onActionClick: React.PropTypes.func
};

export default MapView