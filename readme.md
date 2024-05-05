# An old multi-threaded C application created for school (ZSO)

#### *Przedmiotem projektu jest wielowątkowa aplikacja symulująca zachowanie autobusu. Pasażerowie pojawiają się na różnych przystankach w ciągu działania programu i czekają na autobus.*

### Zasady:
- Pojazd kursuje między kilkoma przystankami w pętli, zatrzymując się by pozwolić pasażerom wsiąść i wysiąść.
- Pasażerowie posiadają z góry ustalony przystanek na którym wysiąść chcą z autobusu.
- Pasażer może wsiąść **wyłącznie** gdy autobus znajdzie się na przystanku na którym czeka i tylko gdy pojazd ma on miejsce go zabrać na pokład.
- Wysiąść mogą **wyłącznie** gdy autobus znajdzie się na pożądanym przystanku, lub na przystanku 0 jeżeli autobus skończy ostatni bieg.
- Autobus wykonuje z góry założoną liczbę pętli po przystankach, po czym kończy bieg.

### Wymagania techniczne:
- Zarówno autobus jak i pasażerowie reprezentowani są przez wątki
- Należy skorzystać z mechanizmów mutex i zmiennych decyzyjnych do kontroli ruchu i komunikacji między wątkami
- Stałe takie jak łączna liczba przystanków, częstotliwość pojawiania się pasażerów, pojemność autobusu, etc. powinny być konfigurowalne (`#define`)
- Należy zminimalizować liczbę użytych bibliotek zewnętrznych. Wyjątkiem jest `pthread`.
- Należy unikać elementów losowości
- Należy zapewnić pełne bezpieczeństwo wątków