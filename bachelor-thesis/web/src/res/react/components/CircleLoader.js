import React from 'react'
import classNames from 'classnames'

const CircleLoader = (props) => {
    return (
        <div className={classNames(props.className, 'progress-circle-indeterminate', 'progress-circle-' + props.color, { 'hide': !props.visible })}></div>
    );
};

CircleLoader.propTypes = {
    className: React.PropTypes.string,
    color: React.PropTypes.string,
    hide: React.PropTypes.bool
};

CircleLoader.defaultProps = {
    color: 'success',
    hide: false
};

export default CircleLoader