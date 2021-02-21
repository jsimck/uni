import React from 'react'

const PanelHeader = (props) => {
    return (
        <div className="panel-heading separator">
            <h3 className="panel-title">
                {props.title}
            </h3>
        </div>
    );
};

PanelHeader.propTypes = {
    title: React.PropTypes.string
};

export default PanelHeader;