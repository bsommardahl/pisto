open Jest;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

open Expect;

describe("OrderItemNote", () => {
  describe("when there are no notes", () =>
    test("it renders the expected number of notes", () => {
      let wrapper = Enzyme.shallow(<DisplayOrderItemNotes notes=[] />);
      let notesNodes = wrapper |> Enzyme.find("ul") |> Enzyme.children;
      expect(Enzyme.length(notesNodes)) |> toBe(0);
    })
  );
  describe("when there are three notes", () =>
    test("it renders the expected number of notes", () => {
      let notes: list(OrderItemNote.t) = [
        {id: "1", value: "Test note 1"},
        {id: "2", value: "Test note 2"},
        {id: "3", value: "Test note 3"},
      ];
      let wrapper = Enzyme.shallow(<DisplayOrderItemNotes notes />);
      let notesNodes = wrapper |> Enzyme.find("ul") |> Enzyme.children;
      expect(Enzyme.length(notesNodes)) |> toBe(notes |> List.length);
    })
  );
  describe("when there are three td elements", () => {
    test("it renders the correct amount of td elements", () => {
      let wrapper = Enzyme.shallow(<DisplayOrderItemNotes notes=[] />);
      let tdNodes = wrapper |> Enzyme.find("td");
      expect(Enzyme.length(tdNodes)) |> toBe(3);
    });
    /* test("it renders the last td element with a colSpan equal to two", () => {
      let wrapper = Enzyme.shallow(<DisplayOrderItemNotes notes=[] />);
      let tdNode = wrapper |> Enzyme.find("td") |> Enzyme.last;
      expect(true) |> toBe(true);
    }); */
  });
});