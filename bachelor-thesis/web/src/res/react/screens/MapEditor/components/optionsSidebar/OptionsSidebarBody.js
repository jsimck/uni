import React from 'react'
import { Row } from 'react-bootstrap'

const OptionsSidebarBody = (props) => {
    return (
        <Row>
            {props.children}
        </Row>
    );
};

OptionsSidebarBody.propTypes = {
    children: React.PropTypes.any
};

export default OptionsSidebarBody