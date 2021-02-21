import React, { Component, StyleSheet, View } from 'react-native'
import Camera from 'react-native-camera'

class CameraView extends Component
{
    render() {
        return (
            <Camera
                style={styles.preview}
                onBarCodeRead={(event) => this.props.onBarCodeRead(event)}
                aspect={Camera.constants.Aspect.fill}>
            </Camera>
        );
    }
}

const styles = StyleSheet.create({
    preview: {
        flex: 1,
        justifyContent: 'flex-end',
        alignItems: 'center'
    }
});

CameraView.propTypes = {
    // Functions
    onBarCodeRead: React.PropTypes.func
};

export default CameraView