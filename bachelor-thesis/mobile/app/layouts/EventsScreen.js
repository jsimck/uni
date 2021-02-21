import React, { Component, Platform, View, Text, StyleSheet, ListView, RefreshControl, Alert } from 'react-native'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { eventsSelector } from '../selectors/eventsSelector'
import { fetchEvents } from '../actions/eventsActions'
import NotificationAlert from '../components/NotificationAlert'
import QrReader from './QrReaderModal'
import EventScreen from './EventScreen'
import EventsRow from './EventsRow'

class EventsScreen extends Component {
    constructor(props) {
        super(props);

        // Create data source
        var ds = new ListView.DataSource({
            rowHasChanged: (r1, r2) => r1 != r2,
            sectionHeaderHasChanged: (s1, s2) => s1 != s2
        });

        // Default state
        this.state = {
            dataSource: ds.cloneWithRowsAndSections(props.events)
        };
    }

    getSortedData(data) {
        var newData = [];
        var today = new Date();

        // Filter upcoming events
        newData['upcoming'] = _.filter(data, (event) => {
            return today < new Date(event.startDate);
        });

        // Filter ongoing events
        newData['ongoing'] = _.filter(data, (event) => {
            return (today >= new Date(event.startDate)) && (today <= new Date(event.endDate));
        });

        // Filter old events
        newData['old'] = _.filter(data, (event) => {
            return today > new Date(event.endDate);
        });

        // Sort by each section by start date [descending]
        newData['upcoming'] = (_.sortBy(newData['upcoming'], (o) => new Date(o.startDate))).reverse();
        newData['ongoing'] = (_.sortBy(newData['ongoing'], (o) => new Date(o.startDate))).reverse();
        newData['old'] = (_.sortBy(newData['old'], (o) => new Date(o.startDate))).reverse();

        // Delete empty keyes
        if (newData['upcoming'].length <= 0) {
            delete newData['upcoming'];
        }

        if (newData['ongoing'].length <= 0) {
            delete newData['ongoing'];
        }

        if (newData['old'].length <= 0) {
            delete newData['old'];
        }

        return newData;
    }

    componentDidMount() {
        // Fetch events if they're not cached and there are no errors
        if (!this.props.cached && !this.props.error) {
            this.props.fetchEvents();
        }
    }

    componentWillReceiveProps(nextProps) {
        if (nextProps.events !== this.props.events) {
            this.setState({
                dataSource: this.state.dataSource.cloneWithRowsAndSections(this.getSortedData(nextProps.events))
            });
        }

        // Cache for events expired, so we will fetch them again
        if (!nextProps.cached && !nextProps.error) {
            this.props.fetchEvents();
        }
    }

    handleOnEventPress(event) {
        // Push new screen
        this.props.navigator.push({
            title: event.title,
            name: 'event',
            component: EventScreen,
            data: {
                eventId: event.id
            }
        });
    }

    renderRow(rowData) {
        return <EventsRow onEventPress={(event) => this.handleOnEventPress(event)} event={rowData} />
    }

    renderSectionHeader(sectionData, sectionID) {
        // Define section titles
        var titles = {
            upcoming: 'Nadcházející',
            ongoing: 'Probíhající',
            old: 'Ukončené'
        };

        // Define section styles
        var sectionStyles = {
            upcoming: styles.sectionUpcoming,
            ongoing: styles.sectionOngoing,
            old: styles.sectionOld
        };

        return (
            <View style={[styles.sectionContainer, sectionStyles[sectionID]]}>
                <Text style={styles.sectionText}>{titles[sectionID]}</Text>
            </View>
        )
    }

    render() {
        return (
            <View style={styles.parentContainer}>
                <ListView
                    style={styles.listView}
                    dataSource={this.state.dataSource}
                    renderRow={(rowData) => this.renderRow(rowData)}
                    renderSectionHeader={(sectionData, sectionID) => this.renderSectionHeader(sectionData, sectionID)}
                    automaticallyAdjustContentInsets={false}
                    refreshControl={
                        <RefreshControl
                            refreshing={this.props.refreshing}
                            onRefresh={() => this.props.fetchEvents(true)}
                            tintColor="#21bfbd"
                            enabled={true}
                            colors={['#21bfbd']}
                            progressBackgroundColor="#2d3646"
                        />
                    }
                />
                <QrReader navigator={this.props.navigator} />
            </View>
        );
    }
}

const styles = StyleSheet.create({
    parentContainer: {
        flex: 1,
        marginTop: (Platform.OS === 'ios') ? 64 : 56,
        backgroundColor: '#ffffff'
    },
    sectionUpcoming: {
        backgroundColor: '#48b0f7'
    },
    sectionOngoing: {
        backgroundColor: '#21bfbd'
    },
    sectionOld: {
        backgroundColor: '#f55753'
    },
    sectionText: {
        fontFamily: 'Source Sans Pro',
        color: '#fff',
        fontWeight: 'bold',
        fontSize: 13
    },
    sectionContainer: {
        paddingHorizontal: 18,
        paddingVertical: 6
    },
    container: {
        flex: 1,
        marginTop: (Platform.OS === 'ios') ? 64 : 56
    }
});

EventsScreen.propTypes = {
    navigator: React.PropTypes.object,
    loading: React.PropTypes.bool,
    refreshing: React.PropTypes.bool,
    cached: React.PropTypes.bool,
    error: React.PropTypes.bool,
    events: React.PropTypes.array,

    // Functions
    fetchEvents: React.PropTypes.func
};

export default connect(
    eventsSelector,
    (dispatch) => {
        return bindActionCreators({ fetchEvents }, dispatch);
    }
)(EventsScreen)