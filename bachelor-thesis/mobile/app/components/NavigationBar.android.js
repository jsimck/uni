import React, { View, TouchableOpacity, Text, StyleSheet } from 'react-native'
import Icon from 'react-native-vector-icons/MaterialIcons'

const NavigationBar = {
    LeftButton: function(route, navigator, index, navState) {
        if (index === 0) {
            return (
                <Text style={[styles.navBarText, styles.navBarTitleText, styles.navBarTitleTextPushed]}>
                    {route.title}
                </Text>
            );
        }

        return (
            <TouchableOpacity
                onPress={() => navigator.pop()}
                style={[styles.navBarButton, styles.navBarButtonLeft]}>
                <View style={styles.navBarButtonContainer}>
                    <Icon style={styles.navBarButtonIcon} name="arrow-back" size={25} />
                </View>
            </TouchableOpacity>
        );
    },

    RightButton: function(route, navigator, index, navState) {
        if (index != 0) {
            return null;
        }

        return (
            <TouchableOpacity
                onPress={() => route.data.onQrCodeShow()}
                style={[styles.navBarButton, styles.navBarButtonCamera]}>
                <View style={styles.navBarButtonContainer}>
                    <Icon style={styles.navBarButtonIcon} name="camera" size={25} />
                </View>
            </TouchableOpacity>
        );
    },

    Title: function(route, navigator, index, navState) {
        if (index == 0) {
            return null;
        }

        return (
            <Text numberOfLines={1} style={[styles.navBarText, styles.navBarTitleText]}>
                {route.title}
            </Text>
        );
    }
};

const styles = StyleSheet.create({
    navBar: {
        backgroundColor: '#2d3646'
    },
    navBarText: {
        fontSize: 20,
        color: 'white',
        marginTop: 17
    },
    navBarButtonContainer: {
        flex: 1,
        flexDirection: 'row',
        justifyContent: 'flex-start',
        alignItems: 'center'
    },
    navBarButtonCamera: {
        padding: 4,
        marginTop: 1,
        marginRight: 10
    },
    navBarTitleText: {
        color: 'white',
        fontWeight: '500'
    },
    navBarTitleTextPushed: {
        marginLeft: 13
    },
    navBarButton: {
        marginTop: 3
    },
    navBarButtonLeft: {
        marginTop: 5,
        marginLeft: -3
    },
    navBarButtonRefresh: {
        marginTop: 6,
        marginRight: 5
    },
    navBarButtonIcon: {
        color: '#21bfbd',
        marginTop: 13,
        marginLeft: 15
    }
});

export default NavigationBar