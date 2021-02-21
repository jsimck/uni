import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { RenderInBody, Notification } from '../components'
import { removeNotification } from '../actions/notifications'
import { notificationsSelector } from '../selectors/notifications'

class Notifications extends Component {
    render() {
        return (
            <RenderInBody>
                <div className="pgn-wrapper" data-position={this.props.position}>
                    {this.props.notifications.map((item) => {
                        return (
                            <Notification onRemove={(id) => this.props.removeNotification(id)}
                                          type={item.type} id={item.id} key={item.id}>{item.message}
                            </Notification>
                        )
                    })}
                </div>
            </RenderInBody>
        );
    }
}

Notifications.propTypes = {
    position: React.PropTypes.string,
    notifications: React.PropTypes.array,

    // Functions
    removeNotification: React.PropTypes.func
};

Notifications.defaultProps = {
    position: 'top-right'
};

export default connect(
    notificationsSelector,
    (dispatch) => {
        return bindActionCreators({ removeNotification }, dispatch);
    }
)(Notifications)