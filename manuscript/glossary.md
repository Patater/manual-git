# Glossary

Programming the Nintendo DS, and other embedded hardware systems in general,
requires knowledge of many different concepts the terms used to describe them.
Throughout this manual, we've introduced many terms that you probably have
never encountered, excepting that you might have had other embedded hardware or
video game programming practice.


<a name="affine_transformation_matrix"></a>

### Affine transformation matrix

Affine transformation matrix


<a name="api"></a>

### Application programming interface (API)

Application programming interface


<a name="affine"></a>

### Affine

Affine


<a name="interrupt"></a>

### Interrupt

Interrupt


<a name="irq_handler"></a>

### Interrupt Request Handler (IRQ Handler)

An interrupt handler is a function that is called after an interrupt is
triggered. It is code designed to do something useful as a result of the
interrupt. A common example of an interrupt handler is the VBlank interrupt
handler. Upon entering VBlank, the Nintendo DS will cause the VBlank handler to
run which will often update the graphics engines and/or VRAM to draw new images
onto the display. The updating of the display is tied to the VBlank interrupt
to avoid screen artifacts and to keep screen updating from the software side in
sync with screen updating on the hardware side.

See also [Interrupt](#interrupt), [Vertical blanking period](#vblank)


<a name="vblank"></a>

### Vertical blanking period (VBlank)

Vertical blanking period
