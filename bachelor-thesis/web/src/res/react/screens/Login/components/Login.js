import React, { Component } from 'react'
import { Row, Col } from 'react-bootstrap'
import { Panel, PanelBody, PanelHeader } from '../../../components'
import Notifications from '../../../components/Notifications'
import LoginForm from './LoginForm'

class Login extends Component
{
    render() {
        return (
            <Row className="m-t-100 m-b-100">
                <Col sm={6} smOffset={3}>
                    <Panel>
                        <PanelHeader title="Přihlášení" />
                        <PanelBody>
                            <LoginForm />
                            <Notifications />
                        </PanelBody>
                    </Panel>
                </Col>
            </Row>
        );
    }
}

Login.propTypes = {};
Login.defaultProps = {};

export default Login