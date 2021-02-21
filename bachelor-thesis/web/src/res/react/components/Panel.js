import React from 'react'

const Panel = (props) => {
    return (
        <div className="panel panel-default">
            {props.children}
        </div>
    );
};

Panel.propTypes = {
    children: React.PropTypes.any.isRequired
};

export default Panel;