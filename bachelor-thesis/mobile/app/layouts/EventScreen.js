import React, { Component, Platform, InteractionManager, StyleSheet, View, Text, WebView, ScrollView, Animated, TouchableOpacity, Dimensions } from 'react-native'
import TabNavigator from 'react-native-tab-navigator'
import _ from 'lodash'
import Icon from 'react-native-vector-icons/FontAwesome'
import moment from 'moment'
import momentCs from 'moment/locale/cs'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { fetchMap } from '../actions/eventsActions'
import { setAction } from '../actions/actionActions'
import { eventSelector } from '../selectors/eventSelector'
import RichContent from '../components/RichContent'
import MapView from '../components/MapView'
import ActionScreen from './ActionScreen'
import API from '../services/APIService'

const DATE_HIDE_THRESHOLD = 35;

class EventScreen extends Component {
    constructor(props) {
        super(props);

        this.state = {
            dateVisible: true,
            scrollEnabled: true,
            selectedTab: props.selectedTab || 'informace',
            slideDateAnim: new Animated.Value(0)
        };
    }

    componentDidMount() {
        // Return if there's no mapId or map is already loaded
        if (this.props.event.mapId == null || this.props.map.hasOwnProperty('mapData')) {
            return;
        }

        // Fetch map data if map exists
        InteractionManager.runAfterInteractions(() => {
            this.props.fetchMap(this.props.event.mapId, this.props.event.id);
        });
    }

    handleOnActionClick(action) {
        // No action available
        if (action == null) {
            return;
        }

        // Set selected action
        this.props.setAction(action);

        // Push new view
        this.props.navigator.push({
            title: action.title,
            name: 'action',
            component: ActionScreen
        });
    }

    handleOnScroll(event) {
        // Check if content is bigger than device size
        if (event.nativeEvent.contentSize.height < event.nativeEvent.layoutMeasurement.height) {
            return;
        }

        if (event.nativeEvent.contentOffset.y > DATE_HIDE_THRESHOLD && this.state.dateVisible) {
            // Animate event up (hide) if it passes set threshold
            this.toggleDateContainer(false);
        } else if (event.nativeEvent.contentOffset.y <= DATE_HIDE_THRESHOLD && !this.state.dateVisible) {
            // Animate event down (show) if it passes set threshold
            this.toggleDateContainer(true);
        }
    }

    toggleDateContainer(show) {
        if (show) {
            this.setState({ dateVisible: true });
            Animated.timing(this.state.slideDateAnim, {
                toValue: 0,
                duration: 200
            }).start();
        } else {
            this.setState({ dateVisible: false });
            Animated.timing(this.state.slideDateAnim, {
                toValue: -27,
                duration: 200
            }).start();
        }
    }

    getProgressWidth() {
        // Get screen width - padding
        var screenWidth = Dimensions.get('window').width - (18 * 2);
        return Math.round((this.props.game.qrIds.length / this.props.event.game.qrCount) * screenWidth);
    }

    handleOnExpand(expanded) {
        // Scroll top view to top
        this.refs._scrollView.scrollTo({
            x: 0,
            y: 0,
            animated: true
        });

        // Show date container
        this.toggleDateContainer(true);

        // Disable scroll view if map is expanded so user can navigate the map up and down
        this.setState({
            scrollEnabled: !expanded
        });
    }

    render() {
        return (
            <View style={styles.container}>
                <ScrollView
                    ref="_scrollView"
                    style={styles.scrollViewContainer}
                    scrollEnabled={this.state.scrollEnabled}
                    onScroll={(event) => this.handleOnScroll(event)}
                    scrollEventThrottle={100}
                >
                    {!_.isNull(this.props.event.mapId) &&
                        <MapView
                            source={this.props.map.mapData}
                            position={this.props.position}
                            onExpand={(expanded) => this.handleOnExpand(expanded)}
                            onActionClick={(action) => this.handleOnActionClick(action)}
                        />
                    }

                    <View style={styles.tabBar}>
                        <TouchableOpacity style={styles.tabItem} onPress={() => this.setState({ selectedTab: 'informace' })}>
                            <Text style={[styles.tabTitle, this.state.selectedTab == 'informace'? styles.tabTitleSelected : null]}>Informace</Text>
                        </TouchableOpacity>

                        {!_.isNull(this.props.event.game) &&
                            <TouchableOpacity style={styles.tabItem} onPress={() => this.setState({ selectedTab: 'soutez' })}>
                                <Text style={[styles.tabTitle, this.state.selectedTab == 'soutez'? styles.tabTitleSelected : null]}>Soutěž</Text>
                            </TouchableOpacity>
                        }
                    </View>

                    {this.state.selectedTab == 'informace' &&
                        <View style={styles.innerContainer}>
                            <RichContent content={this.props.event.content} />
                        </View>
                    }

                    {!_.isNull(this.props.event.game) && this.state.selectedTab == 'soutez' &&
                        <View>
                            <View style={[styles.innerContainer, styles.gameProgressContainer]}>
                                <Text style={styles.gameTitle}>{this.props.event.game.title}</Text>

                                {this.props.game.qrIds.length < this.props.event.game.qrCount &&
                                    <View>
                                        <Text style={styles.gameProgressTitle}>Nasbíráno QR kódů:</Text>
                                        <View style={styles.progress}>
                                            <View style={styles.progressBackground}>
                                                <View style={[styles.progressFill, { width: this.getProgressWidth() }]} />
                                            </View>
                                            <Text style={styles.progressTitle}>
                                                {this.props.game.qrIds.length}/{this.props.event.game.qrCount}
                                            </Text>
                                        </View>
                                    </View>
                                }

                                {this.props.game.qrIds.length >= this.props.event.game.qrCount &&
                                    <Text style={styles.gameWonTitle}>{this.props.event.game.winningMessage}</Text>
                                }
                            </View>

                            <View style={styles.innerContainer}>
                                <RichContent content={this.props.event.game.info} />
                            </View>
                        </View>
                    }
                </ScrollView>

                <Animated.View style={[styles.dateContainer, {top: this.state.slideDateAnim}]}>
                    <Text style={styles.date}>
                        <Icon name="calendar-o" size={14} />
                        <Text>  {moment(this.props.event.startDate).locale('cs').format('Do MMMM YYYY')} - {moment(this.props.event.endDate).locale('cs').format('Do MMMM YYYY')}</Text>
                    </Text>
                </Animated.View>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        marginTop: (Platform.OS === 'ios') ? 64 : 56,
        backgroundColor: '#ffffff'
    },
    dateContainer: {
        top: 0,
        left: 0,
        right: 0,
        position: 'absolute',
        backgroundColor: '#2d3646',
        height: 27
    },
    date: {
        color: '#a6acb5',
        textAlign: 'center',
        fontFamily: 'Source Sans Pro',
        fontSize: 12
    },
    scrollViewContainer: {
        paddingTop: 27
    },
    innerContainer: {
        paddingHorizontal: 18,
        paddingVertical: 15
    },
    gameTitle: {
        flex: 1,
        flexDirection: 'row',
        alignSelf: 'center',
        textAlign: 'center',
        color: '#3c4350',
        fontSize: 20,
        fontFamily: 'Source Sans Pro',
        marginBottom: 15
    },
    gameProgressTitle: {
        color: '#3c4350',
        fontFamily: 'Source Sans Pro',
        fontSize: 14,
        marginBottom: 5
    },
    gameWonTitle: {
        color: '#21bfbd',
        textAlign: 'center',
        fontFamily: 'Source Sans Pro',
        fontSize: 14,
        fontWeight: 'bold',
        marginBottom: 5
    },
    tabTitle: {
        color: '#a6acb5',
        fontFamily: 'Source Sans Pro',
        textAlign: 'center',
        fontSize: 15
    },
    tabTitleSelected: {
        color: '#21bfbd'
    },
    tabItem: {
        flex: 1,
        alignSelf: 'center',
        paddingVertical: 17,
        flexDirection: 'column'
    },
    tabBar: {
        flex: 1,
        alignSelf: 'stretch',
        flexDirection: 'row',
        backgroundColor: '#191f28'
    },
    progressFill: {
        backgroundColor: '#21bfbd',
        borderRadius: 6,
        height: 12
    },
    progressBackground: {
        flex: 1,
        flexDirection: 'column',
        backgroundColor: '#999',
        overflow: 'hidden',
        borderRadius: 6,
        height: 12
    },
    progressTitle: {
        color: '#3c4350',
        textAlign: 'center',
        fontFamily: 'Source Sans Pro',
        fontSize: 14,
        marginTop: -4,
        marginLeft: 10
    },
    gameProgressContainer: {
        backgroundColor: '#f3f3f3'
    },
    progress: {
        flex: 1,
        flexDirection: 'row'
    }
});

EventScreen.propTypes = {
    event: React.PropTypes.object,
    game: React.PropTypes.object,
    map: React.PropTypes.object,
    navigator: React.PropTypes.object,

    // Functions
    fetchMap: React.PropTypes.func
};

export default connect(
    eventSelector,
    (dispatch) => {
        return bindActionCreators({ fetchMap, setAction }, dispatch);
    }
)(EventScreen)