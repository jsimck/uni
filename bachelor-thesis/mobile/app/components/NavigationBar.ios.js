import React, { View, TouchableOpacity, Text, StyleSheet } from 'react-native'
import Icon from 'react-native-vector-icons/EvilIcons'
import IonIcon from 'react-native-vector-icons/Ionicons'
import FaIcon from 'react-native-vector-icons/FontAwesome'

const NavigationBar = {
    LeftButton: function(route, navigator, index, navState) {
        if (index === 0 || route.name == 'qrreader') {
            return null;
        }

        return (
            <TouchableOpacity
                onPress={() => navigator.pop()}
                style={[styles.navBarButton, styles.navBarButtonTitleLeft]}>
                <View style={styles.navBarButtonContainer}>
                    <Icon style={styles.navBarButtonIcon} name="chevron-left" size={45} />
                </View>
            </TouchableOpacity>
        );
    },

    RightButton: function(route, navigator, index, navState) {
        if (index === 0) {
            return (
                <TouchableOpacity
                    onPress={() => route.data.onQrCodeShow()}
                    style={[styles.navBarButton, styles.navBarButtonCamera]}>
                    <View style={styles.navBarButtonContainer}>
                        <IonIcon style={styles.navBarButtonIcon} name="ios-barcode" size={25} />
                    </View>
                </TouchableOpacity>
            );
        }
    },

    Title: function(route, navigator, index, navState) {
        return (
            <Text numberOfLines={1} style={[styles.navBarText, styles.navBarTitleText]}>
                {route.title}
            </Text>
        );
    }
};

const styles = StyleSheet.create({
    navBar: {
        backgroundColor: '#242a35'
    },
    navBarText: {
        fontSize: 16,
        marginTop: 12,
        fontFamily: 'Source Sans Pro',
        fontWeight: 'normal',
        color: 'white'
    },
    navBarButtonContainer: {
        flex: 1,
        flexDirection: 'row',
        justifyContent: 'flex-start',
        alignItems: 'center'
    },
    navBarTitleText: {
        color: 'white',
        marginHorizontal: 45,
        marginTop: 12
    },
    navBarButton: {
        marginTop: 5
    },
    navBarButtonTitleLeft: {
        marginLeft: -3
    },
    navBarButtonRight: {
        marginTop: 9,
        marginRight: 14
    },
    navBarButtonCamera: {
        padding: 9,
        marginTop: 0,
        marginRight: 5
    },
    navBarButtonClose: {
        marginTop: 2,
        marginRight: 0,
        padding: 10
    },
    navBarButtonLeft: {
        marginTop: 9,
        marginLeft: 14
    },
    navBarButtonRefresh: {
        marginTop: 6,
        marginRight: 5
    },
    navBarButtonText: {
        fontSize: 16,
        marginLeft: -10,
        marginTop: -3,
        fontFamily: 'Source Sans Pro',
        color: '#21bfbd'
    },
    navBarButtonIcon: {
        color: '#21bfbd'
    }
});

export default NavigationBar