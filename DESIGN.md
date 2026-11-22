# Design

`main.hyp`

```
{
  meta = import("meta.hyp");
  general = import("general.hyp");
};
```

`meta.hyp`

```
{
  id = "example";
  name = "Example";
};
```

`general.hyp`

```
{
  font = {
    default.size = 16;
    monospace.size = font.default.size; // .default.size works too!
  };
};
```

`kitty.hyp`

```
{
  font.size = import("general.hyp").font.monospace.size;
};
```
