import React, { Component, StyleSheet, View, Navigator, TouchableOpacity, StatusBar, Platform } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import { appSelector } from './../selectors/appSelector';
import { showQrReader } from './../actions/qrReaderActions';
import NotificationAlert from '../components/NotificationAlert';
import NavigationBar from '../components/NavigationBar';
import EventScreen from './EventScreen';
import EventsScreen from './EventsScreen';

class App extends Component {
    renderScene(route, navigator) {
        const Component = route.component;
        return (
            <Component navigator={navigator} route={route} {...route.data} />
        );
    }

    configureScene(route) {
        if (!route.sceneConfig) {
            var transition = Navigator.SceneConfigs.FloatFromRight;

            // Change default view transition only for iOS platform
            if (Platform.OS === 'ios') {
                transition.animationInterpolators = Navigator.SceneConfigs.HorizontalSwipeJump.animationInterpolators;
            }

            return transition;
        }

        return route.sceneConfig;
    }

    render() {
        return (
            <View style={styles.container}>
                <StatusBar
                    backgroundColor="#232a36"
                    barStyle="light-content"
                    networkActivityIndicatorVisible={this.props.networkActivity}
                />
                <Navigator
                    initialRoute={{
                        title: 'Události',
                        component: EventsScreen,
                        data: {
                            onQrCodeShow: () => this.props.showQrReader()
                        }
                    }}
                    renderScene={(route, navigator) => this.renderScene(route, navigator)}
                    configureScene={this.configureScene}
                    navigationBar={
                        <Navigator.NavigationBar
                            routeMapper={NavigationBar}
                            style={styles.navBar}
                        />
                    }
                />
                <NotificationAlert />
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1
    },
    navBar: {
        backgroundColor: '#2d3646'
    }
});

App.propTypes = {
    networkActivity: React.PropTypes.bool,

    // Functions
    showQrReader:  React.PropTypes.func
};

export default connect(
    appSelector,
    (dispatch) => {
        return bindActionCreators({ showQrReader }, dispatch);
    }
)(App)