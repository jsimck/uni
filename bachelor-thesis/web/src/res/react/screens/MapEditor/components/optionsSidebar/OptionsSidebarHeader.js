import React from 'react'
import { Row, Col } from 'react-bootstrap'

const OptionsSidebarHeader = (props) => {
    return (
        <Row>
            <Col xs={12}><h6>{props.children}</h6></Col>
        </Row>
    );
};

OptionsSidebarHeader.propTypes = {
    children: React.PropTypes.any
};

export default OptionsSidebarHeader