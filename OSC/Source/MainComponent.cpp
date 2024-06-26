#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);

	addAndMakeVisible(volumeSlider);
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
	Sender.connect("127.0.0.1", 9001);

	volumeSlider.onValueChange = [this]
		{// create and send an OSC message with an address and a float value:
			if (!Sender.send("/juce/rotaryknob", (float)volumeSlider.getValue()))
				showConnectionErrorMessage("Error: could not send OSC message.");
		};

	// Some platforms require permissions to open input channels so request that here
	if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
		&& !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
	{
		juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
			[&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}
}

MainComponent::~MainComponent()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio();
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	// Your audio-processing code goes here!

	// For more details, see the help for AudioProcessor::getNextAudioBlock()

	// Right now we are not producing any data, in which case we need to clear the buffer
	// (to prevent the output of random noise)
	bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.

	// For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
	volumeSlider.setBounds(10, 10, 180, 180);
}

