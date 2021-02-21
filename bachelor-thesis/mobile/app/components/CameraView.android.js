import React, { Component, StyleSheet, View } from 'react-native'
import BarcodeScanner from 'react-native-barcodescanner'

class CameraView extends Component
{
    render() {
        return (
            <BarcodeScanner
                style={styles.preview}
                onBarCodeRead={(event) => this.props.onBarCodeRead(event)}
                cameraType="back">
            </BarcodeScanner>
        );
    }
}

const styles = StyleSheet.create({
    preview: {
        flex: 1,
        flexDirection: 'row',
        alignSelf: 'stretch',
        alignItems: 'center'
    }
});

CameraView.propTypes = {
    // Functions
    onBarCodeRead: React.PropTypes.func
};

export default CameraView