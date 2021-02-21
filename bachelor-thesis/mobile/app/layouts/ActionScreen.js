import React, { Component, Platform, StyleSheet, View, ScrollView, Text } from 'react-native'
import { connect } from 'react-redux'
import { actionSelector } from '../selectors/actionSelector'
import RichContent from '../components/RichContent'

class ActionScreen extends Component {
    render() {
        return (
            <ScrollView style={styles.container}>
                <RichContent content={this.props.action.content} />
            </ScrollView>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        marginTop: (Platform.OS === 'ios') ? 64 : 56,
        backgroundColor: '#ffffff',
        paddingHorizontal: 18,
        paddingVertical: 15
    }
});

ActionScreen.propTypes = {
    action: React.PropTypes.object
};

export default connect(actionSelector)(ActionScreen)