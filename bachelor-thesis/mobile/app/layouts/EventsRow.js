import React, { Component, StyleSheet, View, Text, TouchableOpacity, Image } from 'react-native'
import _ from 'lodash'
import Icon from 'react-native-vector-icons/FontAwesome'
import moment from 'moment'
import RichContent from '../components/RichContent'

class EventsRow extends Component
{
    getRowContainerStyles(date) {
        if (new Date() > new Date(date)) {
            return [styles.rowContainer, styles.oldEvent];
        }

        return styles.rowContainer;
    }

    render() {
        // Check if event is not null
        if (_.isNull(this.props.event)) {
            return null;
        }

        return (
            <View style={this.getRowContainerStyles(this.props.event.endDate)}>
                <TouchableOpacity style={styles.rowButtonContainer} onPress={() => this.props.onEventPress(this.props.event)}>
                    <Text style={styles.title}>{this.props.event.title}</Text>

                    <Text style={styles.date}>
                        <Icon name="calendar-o" size={14} />
                        <Text>  {moment(this.props.event.startDate).locale('cs').format('Do MMMM YYYY')} - {moment(this.props.event.endDate).locale('cs').format('Do MMMM YYYY')}</Text>
                    </Text>

                    <Text style={styles.description}>
                        {this.props.event.description}
                    </Text>
                </TouchableOpacity>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    rowContainer: {
        borderBottomColor: '#dedede',
        borderBottomWidth: StyleSheet.hairlineWidth
    },
    oldEvent: {
        opacity: 0.6
    },
    rowButtonContainer: {
        flex: 1,
        flexDirection: 'column',
        backgroundColor: '#ffffff',
        paddingHorizontal: 18,
        paddingVertical: 15
    },
    title: {
        flex: 1,
        color: '#3c4350',
        fontFamily: 'Source Sans Pro',
        fontSize: 18
    },
    description: {
        flex: 1,
        color: '#696d6f',
        fontFamily: 'Source Sans Pro',
        fontSize: 14,
        marginTop: 0,
        marginBottom: 0
    },
    date: {
        flex: 1,
        color: '#9da1a7',
        fontFamily: 'Source Sans Pro',
        fontSize: 14,
        marginVertical: 5
    }
});

EventsRow.propTypes = {
    event: React.PropTypes.object,
    onEventPress: React.PropTypes.func
};

export default EventsRow