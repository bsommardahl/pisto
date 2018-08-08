[@bs.send]
external dive : Enzyme.shallowWrapper => Enzyme.shallowWrapper = "dive";

open Jest;
open Expect;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("NotesInput", () => {
  describe("when input has a placeholder", () =>
    test("it should show placeholder's value", () => {
      open Js.Types;
      let wrapper = Enzyme.shallow(<NotesInput onFinish=(_ => ()) />);
      let input = wrapper |> Enzyme.find("input") |> Enzyme.first;

      let result =
        switch (Enzyme.jsPropFor("placeholder", input) |> classify) {
        | JSString(placeholder) => placeholder
        | _ => ""
        };
      expect(result) |> toBe("Notes...");
    })
  );
  describe("when input is empty", () =>
    test("it should disable add button", () => {
      open Js.Types;
      let wrapper = Enzyme.shallow(<NotesInput onFinish=(_ => ()) />);
      let button =
        wrapper
        |> Enzyme.find("Button")
        |> dive
        |> Enzyme.find("button")
        |> Enzyme.first;
      let result =
        switch (Enzyme.jsPropFor("disabled", button) |> classify) {
        | JSTrue => true
        | _ => false
        };
      expect(result) |> toBe(true);
    })
  );
});