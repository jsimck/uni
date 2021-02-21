import React, { Component, StyleSheet, View, Text, Linking } from 'react-native'
import HtmlRender from 'react-native-html-render'
import _ from 'lodash'

class RichContent extends Component
{
    linkHandler(url) {
        Linking.openURL(url);
    }

    render() {
        return (
            <HtmlRender
                value={this.props.content}
                stylesheet={styles}
                onLinkPress={(url) => this.linkHandler(url)}
            />
        );
    }
}

const styles = StyleSheet.create({
    baseStyle: {
        fontFamily: 'Source Sans Pro',
        color: '#696d6f'
    },
    p: {
        lineHeight: 21,
        fontSize: 15
    },
    pwrapper: {
        marginTop: 0,
        marginBottom: 10
    },
    h1: {
        color: '#3c4350',
        fontSize: 30
    },
    h1wrapper: {
        marginTop: 0,
        marginBottom: 10
    },
    h2: {
        color: '#3c4350',
        fontSize: 26
    },
    h2wrapper: {
        marginTop: 0,
        marginBottom: 10
    },
    h3: {
        color: '#3c4350',
        fontSize: 20
    },
    h3wrapper: {
        marginTop: 0,
        marginBottom: 10
    },
    blockquote: {
        fontSize: 15,
        fontFamily: 'Raleway',
        lineHeight: 22
    },
    blockquotewrapper: {
        paddingLeft: 20,
        borderLeftColor: '#21bfbd',
        borderLeftWidth: 3,
        marginTop: 0,
        marginBottom: 10
    },
    del: {
        textDecorationLine: 'line-through'
    },
    strong: {
        fontWeight: 'bold'
    },
    a: {
        color: '#21bfbd',
        textDecorationLine: 'underline'
    },
    ins: {
        textDecorationLine: 'underline'
    },
    em: {
        fontStyle: 'italic'
    },
    code: {
        fontFamily: 'Menlo'
    },
    li: {
        fontSize: 15,
        color: '#696d6f'
    },
    liwrapper: {
        paddingLeft: 5,
        marginTop: 2,
        marginBottom: 2
    },
    ulwrapper: {
        marginTop: 0,
        marginBottom: 10
    }
});

RichContent.propTypes = {
    content: React.PropTypes.string
};

export default RichContent