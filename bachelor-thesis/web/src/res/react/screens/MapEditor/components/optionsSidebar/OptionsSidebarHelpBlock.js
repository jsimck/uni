import React from 'react'

const OptionsSidebarHelpBlock = (props) => {
    return (
        <p style={props.style} className="help-block fs-11">
            {props.children}
        </p>
    );
};

OptionsSidebarHelpBlock.propTypes = {
    children: React.PropTypes.any,
    style: React.PropTypes.object
};

export default OptionsSidebarHelpBlock