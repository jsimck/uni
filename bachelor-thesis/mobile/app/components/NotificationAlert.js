import React, { Component, StyleSheet, View, Alert } from 'react-native'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import _ from 'lodash'
import { notificationAlertSelector } from '../selectors/notificationAlertSelector'
import { closeAlert } from '../actions/notificationAlertActions'

class NotificationAlert extends Component {
    render() {
        if (!_.isEmpty(this.props.alert)) {
            Alert.alert(
                this.props.alert.title,
                this.props.alert.message,
                [{
                    text: 'Zavřít',
                    onPress: () => this.props.closeAlert()
                }]
            );
        }

        // We're not rendering anything, only showing alert
        return false;
    }
}

NotificationAlert.propTypes = {
    alert: React.PropTypes.object,

    // Functions
    closeAlert: React.PropTypes.func
};

export default connect(
    notificationAlertSelector,
    (dispatch) => {
        return bindActionCreators({ closeAlert }, dispatch);
    }
)(NotificationAlert)