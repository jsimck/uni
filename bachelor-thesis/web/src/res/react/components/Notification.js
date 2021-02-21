import React, { Component } from 'react'
import classNames from 'classnames'
import { removeNotification } from '../actions/notifications'

class Notification extends Component {
    componentDidMount() {
        // Remove after some time
        if (this.props.delay > 0) {
            setTimeout(() => {
                this.props.onRemove(this.props.id);
            }, this.props.delay);
        }
    }

    render() {
        return (
            <div className={classNames('pgn', 'pgn-' + this.props.style)}>
                <div className={classNames('alert', 'alert-' + this.props.type)}>
                    {this.props.dismiss &&
                        <button onClick={() => this.props.onRemove(this.props.id)} className="close">
                            <span aria-hidden="true">×</span>
                            <span className="sr-only">Zavřít</span>
                        </button>
                    }
                    <span>{this.props.children}</span>
                </div>
            </div>
        );
    }
}

Notification.propTypes = {
    id: React.PropTypes.number,
    delay: React.PropTypes.number,
    type: React.PropTypes.string,
    style: React.PropTypes.string,
    dismiss: React.PropTypes.bool,
    onRemove: React.PropTypes.func,
    children: React.PropTypes.any.isRequired
};

Notification.defaultProps = {
    style: 'flip',
    type: 'success',
    dismiss: true,
    delay: 3000
};

export default Notification