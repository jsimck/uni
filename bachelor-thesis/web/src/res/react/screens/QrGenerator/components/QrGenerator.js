import React, { Component } from 'react'
import { Row, Col, Button, Input } from 'react-bootstrap'
import QRCode from 'qrcode.react'
import { Panel, PanelBody, PanelHeader } from '../../../components'

class QRGenerator extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            qrValue: ''
        };
    }

    linkState(key, value) {
        this.setState({
            [key]: value
        });
    }

    handleOnDownloadClick() {
        // Create invisible link to download qr code as img
        var downloadLink = document.createElement('a');
        downloadLink.href = document.querySelector('#qrCodeGeneratorWrapper').children[0].toDataURL();
        downloadLink.download = 'qrcode';

        // Attach to dom and remove after click
        document.body.appendChild(downloadLink);
        downloadLink.click();
        document.body.removeChild(downloadLink);
    }

    render() {
        return (
            <Row>
                <Col sm={6} smOffset={3}>
                    <Panel>
                        <PanelHeader title="QR Generátor" />
                        <PanelBody className="p-b-10 p-t-20" >
                            <div className="m-b-20 text-center" id="qrCodeGeneratorWrapper">
                                <QRCode size={400} value={this.state.qrValue} />
                            </div>
                            <Input
                                type="Text"
                                style={{margin: 0}}
                                value={this.state.qrValue}
                                onChange={(event) => this.linkState('qrValue', event.target.value)}
                                label={null}
                                buttonAfter={
                                    <Button bsStyle="primary" onClick={() => this.handleOnDownloadClick()}>
                                        <i className="fa fa-download"></i> Stáhnout
                                    </Button>
                                }
                                placeholder="http://"
                            />
                        </PanelBody>
                    </Panel>
                </Col>
            </Row>
        );
    }
}

QRGenerator.propTypes = {};
QRGenerator.defaultProps = {};

export default QRGenerator