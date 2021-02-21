import React  from 'react'
import classNames from 'classnames'
import { Button } from 'react-bootstrap'

const ButtonLoading = (props) => {
    return (
        <Button bsStyle={props.bsStyle} {...props.buttonProps} disabled={props.disabled} onClick={props.onClick} className={classNames('btn-loading', props.className, {'loading': props.loading})}>
            <span className="btn-text">{props.children}</span>
            <i className={classNames('btn-icon fa-spin', props.icon)}></i>
        </Button>
    );
};

ButtonLoading.propTypes = {
    icon: React.PropTypes.string,
    children: React.PropTypes.any.isRequired,
    bsStyle: React.PropTypes.string,
    className: React.PropTypes.string,
    buttonProps: React.PropTypes.object,
    disabled: React.PropTypes.bool,
    loading: React.PropTypes.bool,
    onClick: React.PropTypes.func
};

ButtonLoading.defaultProps = {
    icon: 'fa fa-refresh',
    bsStyle: 'primary',
    disabled: false,
    loading: false
};

export default ButtonLoading