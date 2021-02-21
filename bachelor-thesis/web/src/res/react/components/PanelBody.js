import React from 'react'
import classNames from 'classnames'

const PanelBody = (props) => {
    return (
        <div style={{...props.style}} className={classNames('panel-body', props.className)}>
            {props.children}
        </div>
    );
};

PanelBody.propTypes = {
    className: React.PropTypes.string,
    children: React.PropTypes.any.isRequired
};

export default PanelBody;