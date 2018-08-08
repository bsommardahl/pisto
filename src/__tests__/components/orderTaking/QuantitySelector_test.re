open Jest;
open Expect;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("QuantitySelector", () =>
  describe("when there are two buttons", () =>
    test("it renders the correct amount of buttons", () => {
      let wrapper =
        Enzyme.shallow(<QuantitySelector value=2 onChange=(_ => ()) />);
      let buttons = wrapper |> Enzyme.find("Button");
      expect(Enzyme.length(buttons)) |> toBe(2);
    })
  )
);